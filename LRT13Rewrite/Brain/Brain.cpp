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

Brain::Brain()
{
	// Create joystick input processors
	m_inputs.push_back(new DrivetrainInputs());
	m_inputs.push_back(new CollectorRollersInputs());
	
	// Create automation tasks
	Automation *auton = new Autonomous();
	Automation *climb = new Climb();
	Automation *collectorDown = new CollectorSlide(true);
	Automation *collectorUp = new CollectorSlide(false);
	
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

	// Define actions between blocked tasks
	m_blockedActions[auton][climb] = OVERRIDE;
	m_blockedActions[climb][auton] = ABORT_SELF;
	m_blockedActions[collectorDown][collectorUp] = ABORT_OTHER;
	m_blockedActions[collectorUp][collectorDown] = ABORT_SELF;
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
}

void Brain::Update()
{
 	ProcessAutomationTasks();
	
	ProcessInputs();
	
	Automation::DeallocateAll();
	
	for (vector<Event*>::iterator it = Event::event_vector.begin(); it < Event::event_vector.end(); it++)
	{
		(*it)->Update();
	}
}

void Brain::ProcessAutomationTasks()
{
	// Try to start queued tasks
	for (map<Automation*, Event*>::iterator a = m_queuedTasks.begin(); a != m_queuedTasks.end(); a++)
	{
	    if (find(m_runningTasks.begin(), m_runningTasks.end(), a->first) == m_runningTasks.end()) // If task isn't running
	    {
		    Automation::Status ret = a->first->Start(a->second);
		    if (ret == Automation::SUCCESS)
		    {
        		m_runningTasks.push_back(a->first);
		        m_queuedTasks.erase(a);
		    }
        }
	}
	
	// Try to abort cleaning up tasks
	for (map<Automation*, Event*>::iterator a = m_abortingTasks.begin(); a != m_abortingTasks.end(); a++)
	{
	    if (find(m_runningTasks.begin(), m_runningTasks.end(), a->first) != m_runningTasks.end()) // If task is running
	    {
		    Automation::Status ret = a->first->Abort(a->second);
		    if (ret == Automation::SUCCESS)
		    {
		        m_runningTasks.remove(a->first);
		        m_abortingTasks.erase(a);
		    }
        }
	}
	
	// Start/Abort/Continue tasks which had their events just fired
	for (vector<Event*>::iterator it = Event::event_vector.begin(); it < Event::event_vector.end(); it++)
	{
		if ((*it)->Fired())
		{
		    // Tasks started by this event
        	for (vector<Automation*>::iterator a = (*it)->GetStartListeners().begin(); a < (*it)->GetStartListeners().end(); a++)
        	{
        		if (find(m_runningTasks.begin(), m_runningTasks.end(), *a) == m_runningTasks.end() || (*a)->IsRestartable()) // If task isn't running or is restartable
        		{
        			Automation::Status ret = (*a)->Start(*it);
        			if (ret == Automation::SUCCESS)
        				m_runningTasks.push_back(*a);
        			else if (ret == Automation::WAIT)
        				m_queuedTasks.insert(pair<Automation*, Event*>(*a, *it));
        		}
        	}
        	
        	// Tasks aborted by this event
        	for (vector<Automation*>::iterator a = (*it)->GetAbortListeners().begin(); a < (*it)->GetAbortListeners().end(); a++)
        	{
        	    if (find(m_runningTasks.begin(), m_runningTasks.end(), *a) != m_runningTasks.end()
        	            && m_abortingTasks.find(*a) == m_abortingTasks.end()) // If task is running and not cleaning up
        	    {
        		    Automation::Status ret = (*a)->Abort(*it);
        		    if (ret == Automation::SUCCESS)
        		        m_runningTasks.remove(*a);
        		    else if (ret == Automation::CLEANING_UP)
        		        m_abortingTasks.insert(pair<Automation*, Event*>(*a, *it));
        		}
        	}

        	// Tasks continued by this event
        	for (vector<Automation*>::iterator a = (*it)->GetContinueListeners().begin(); a < (*it)->GetContinueListeners().end(); a++)
        	{
        	    if (find(m_runningTasks.begin(), m_runningTasks.end(), *a) != m_runningTasks.end()) // If task is running
        	    {
        		    (*a)->Continue(*it);
        		}
        	}
		}
	}
	
    // Update running tasks
    for (list<Automation*>::iterator a = m_runningTasks.begin(); a != m_runningTasks.end(); a++)
    {
	    Automation::Status ret = (*a)->Update();
	    if (ret == Automation::COMPLETED)
	    {
	    	if (m_abortingTasks.find(*a) != m_abortingTasks.end())
	    		m_abortingTasks.erase(*a);
	    	m_runningTasks.erase(a++);
	    	a--;
	    }
	    else if (ret == Automation::RESOURCE_BUSY)
	    {
	    	// Perform appropriate actions on blocking tasks
	    	bool allowOverride = true;
	        for (set<Automation*>::iterator it = (*a)->GetBlockingTasks().begin(); it != (*a)->GetBlockingTasks().end(); it++)
	        {
	        	if (m_blockedActions.find(*a) != m_blockedActions.end() && m_blockedActions[*a].find(*it) != m_blockedActions[*a].end())
	        	{
		        	if (m_blockedActions[*a][*it] == ABORT_SELF)
		        	{
		        		m_abortingTasks.insert(pair<Automation*, Event*>(*a, NULL));
		        		allowOverride = false;
		        		break;
		        	}
		        	else if (m_blockedActions[*a][*it] == IGNORE)
		        	{
		        		allowOverride = false;
		        	}
	        	}
	        	else
	        		allowOverride = false;
	        }
	        if (allowOverride) // All blocking tasks are set to be aborted or overriden
	        {
	    		set<Automation*> overrideTasks;
		        for (set<Automation*>::iterator it = (*a)->GetBlockingTasks().begin(); it != (*a)->GetBlockingTasks().end(); it++)
		       	{
		        	if (m_blockedActions[*a][*it] == ABORT_OTHER)
		        	{
		        		if (m_abortingTasks.find(*it) == m_abortingTasks.end())
		        			m_abortingTasks.insert(pair<Automation*, Event*>(*it, NULL));
		        	}
		        	else if (m_blockedActions[*a][*it] == OVERRIDE)
		        	{
		        		overrideTasks.insert(*it);
		        	}
		        }
		        // Find position of earliest running task to override
		        list<Automation*>::iterator overrideInsert = m_runningTasks.end();
			    for (list<Automation*>::iterator it = m_runningTasks.begin(); it != m_runningTasks.end(); it++)
			    {
			    	if (overrideTasks.find(*it) != overrideTasks.end())
			    	{
			    		overrideInsert = it;
			    		break;
			    	}
			    }
			    list<Automation*>::iterator current = a;
			    current++;
			    if (overrideInsert != m_runningTasks.end())
    				m_runningTasks.splice(overrideInsert, m_runningTasks, a, current); // Move current task in front of all tasks to override
	        }
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
