#include "Brain.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"
#include <fstream>
#include <sstream>
#include "../RobotState.h"
#include "../DriverStation/LRTDriverStation.h"
#include "../Config/DriverStationConfig.h"

#include "InputProcessor/DrivetrainInputs.h"
#include "InputProcessor/CollectorRollersInputs.h"

#include "Automation/Autonomous.h"
#include "Automation/Climb.h"
#include "Automation/CollectorSlide.h"
#include "Automation/Drive.h"
#include "Automation/Turn.h"
#include "Automation/Pause.h"
#include "Automation/Parallel.h"
#include "Automation/Sequential.h"
#include "Automation/Repeating.h"

#include "Events/GameModeChangeEvent.h"
#include "Events/JoystickMovedEvent.h"
#include "Events/JoystickPressedEvent.h"
#include "Events/JoystickHeldEvent.h"
#include "Events/JoystickReleasedEvent.h"
#include "Events/MultipleEvent.h"

Brain* Brain::m_instance = NULL;

Brain* Brain::Instance()
{
	if (m_instance == NULL)
		m_instance = new Brain();
	return m_instance;
}

void Brain::Initialize()
{
	if (m_instance == NULL)
		m_instance = new Brain();
}

void Brain::Finalize()
{
	delete m_instance;
	m_instance = NULL;
}

Brain::Brain() :
	Loggable("Brain")
{
	// Create joystick input processors
	m_inputs.push_back(new DrivetrainInputs());
	m_inputs.push_back(new CollectorRollersInputs());
	
	// Create automation tasks
	Automation *auton = new Autonomous();
	Automation *climb = new Climb();
	Automation *collectorDown = new CollectorSlide(true);
	Automation *collectorUp = new CollectorSlide(false);
	Parallel *positionHold = new Parallel("PositionHold");
	positionHold->AddAutomation(new Drive(0.0));
	positionHold->AddAutomation(new Turn(0.0));
	positionHold->AddAutomation(new Repeating("RepeatPause", new Pause(0.0)));
	m_automation.push_back(auton);
	m_automation.push_back(climb);
	m_automation.push_back(collectorDown);
	m_automation.push_back(collectorUp);
	m_automation.push_back(positionHold);
	
	// Create events to be used
	Event *toAuto = new GameModeChangeEvent(RobotState::AUTONOMOUS);
	Event *toDisabled = new GameModeChangeEvent(RobotState::DISABLED);
	Event *driverStickMoved = new JoystickMovedEvent(LRTDriverStation::Instance()->GetDriverStick());
	Event *operatorStickMoved = new JoystickMovedEvent(LRTDriverStation::Instance()->GetOperatorStick());
	Event *driverStickPressed = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick());
	Event *operatorStickPressed = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick());
	Event *climb_button_held = new JoystickHeldEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::CONTINUE_CLIMB, 25);
	Event *abort_climb = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::ABORT_CLIMB);
	Event *start_climb = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::START_CLIMB);
	Event *continue_climb = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::CONTINUE_CLIMB);
	Event *collector_down_driver = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::COLLECTOR_SLIDE);
	Event *collector_down_operator = new JoystickPressedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::COLLECTOR_DOWN_NO_MOTOR);
	MultipleEvent *collector_up = new MultipleEvent();
	collector_up->AddEvent(new JoystickReleasedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::COLLECTOR_SLIDE));
	collector_up->AddEvent(new JoystickReleasedEvent(LRTDriverStation::Instance()->GetOperatorStick(), DriverStationConfig::JoystickButtons::COLLECTOR_DOWN_NO_MOTOR));
	Event *position_hold_start = new JoystickPressedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::STOP_ROBOT);
	Event *position_hold_abort = new JoystickReleasedEvent(LRTDriverStation::Instance()->GetDriverStick(), DriverStationConfig::JoystickButtons::STOP_ROBOT);
	
	// Map events to tasks to start/abort/continue
	toAuto->AddStartListener(auton);
	driverStickMoved->AddAbortListener(auton);
	operatorStickMoved->AddAbortListener(auton);
	driverStickPressed->AddAbortListener(auton);
	operatorStickPressed->AddAbortListener(auton);
	climb_button_held->AddStartListener(climb);
	toDisabled->AddAbortListener(climb);
	abort_climb->AddAbortListener(climb);
	start_climb->AddContinueListener(climb);
	continue_climb->AddContinueListener(climb);
	collector_down_driver->AddStartListener(collectorDown);
	collector_down_operator->AddStartListener(collectorDown);
	collector_up->AddAbortListener(collectorDown);
	collector_up->AddStartListener(collectorUp);
	collector_down_driver->AddAbortListener(collectorUp);
	collector_down_operator->AddAbortListener(collectorUp);
	position_hold_start->AddStartListener(positionHold);
	position_hold_abort->AddAbortListener(positionHold);
}

Brain::~Brain()
{
	for (vector<Event*>::iterator it = Event::event_vector.begin(); it < Event::event_vector.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	for (vector<InputProcessor*>::iterator it = m_inputs.begin(); it < m_inputs.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	for (vector<Automation*>::iterator it = Automation::automation_vector.begin(); it < Automation::automation_vector.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
}

void Brain::Update()
{
 	ProcessAutomationTasks();
	
	ProcessInputs();
	
	for (vector<Event*>::iterator it = Event::event_vector.begin(); it < Event::event_vector.end(); it++)
	{
		(*it)->Update();
	}
}

void Brain::ProcessAutomationTasks()
{
	// Try to start queued tasks
	for (map<Automation*, Event*>::iterator a = m_waitingTasks.begin(); a != m_waitingTasks.end(); a++)
	{
	    if (find(m_runningTasks.begin(), m_runningTasks.end(), a->first) == m_runningTasks.end()) // If task isn't running
	    {
			if (a->first->CheckResources())
			{
				bool ret = a->first->StartAutomation(a->second);
				if (ret)
				{
					m_runningTasks.push_back(a->first);
					m_waitingTasks.erase(a);
				}
			}
        }
	}
	
	// Start/Abort/Continue tasks which had their events fired
	for (vector<Event*>::iterator it = Event::event_vector.begin(); it < Event::event_vector.end(); it++)
	{
		if ((*it)->Fired())
		{
        	// Tasks aborted by this event
        	for (vector<Automation*>::iterator a = (*it)->GetAbortListeners().begin(); a < (*it)->GetAbortListeners().end(); a++)
        	{
        	    if (find(m_runningTasks.begin(), m_runningTasks.end(), *a) != m_runningTasks.end()) // If task is running
        	    {
        	    	bool ret = (*a)->AbortAutomation(*it);
        		    if (ret)
        		    {
        		    	(*a)->DeallocateResources();
        		        m_runningTasks.remove(*a);
        		    }
        		}
        	}
        	
		    // Tasks started by this event
        	for (vector<Automation*>::iterator a = (*it)->GetStartListeners().begin(); a < (*it)->GetStartListeners().end(); a++)
        	{
        		if (find(m_runningTasks.begin(), m_runningTasks.end(), *a) == m_runningTasks.end() || (*a)->IsRestartable()) // If task isn't running or is restartable
        		{
        			if ((*a)->CheckResources())
        			{
						bool ret = (*a)->StartAutomation(*it);
						if (ret)
							m_runningTasks.push_back(*a);
        			}
        			else
        			{
        				if ((*a)->QueueIfBlocked())
        					m_waitingTasks.insert(pair<Automation*, Event*>(*a, *it));
        			}
        		}
        	}

        	// Tasks continued by this event
        	for (vector<Automation*>::iterator a = (*it)->GetContinueListeners().begin(); a < (*it)->GetContinueListeners().end(); a++)
        	{
        	    if (find(m_runningTasks.begin(), m_runningTasks.end(), *a) != m_runningTasks.end()) // If task is running
        	    {
        		    (*a)->ContinueAutomation(*it);
        		}
        	}
		}
	}
	
    // Update running tasks
    for (list<Automation*>::iterator a = m_runningTasks.begin(); a != m_runningTasks.end(); a++)
    {
    	bool complete = (*a)->Update();
	    if (complete)
	    {
	    	(*a)->DeallocateResources();
	    	m_runningTasks.erase(a++);
	    	a--;
	    }
    }
}

void Brain::ProcessInputs()
{
	for (vector<InputProcessor*>::iterator it = m_inputs.begin(); it < m_inputs.end(); it++)
	{
		if ((*it)->CheckResources())
		{
			(*it)->Update();
		}
	}
}

void Brain::Log()
{
	for (vector<Automation*>::iterator it = m_automation.begin(); it < m_automation.end(); it++)
	{
		LogToFile(find(m_runningTasks.begin(), m_runningTasks.end(), *it) != m_runningTasks.end(), (*it)->GetName());
	}
}
