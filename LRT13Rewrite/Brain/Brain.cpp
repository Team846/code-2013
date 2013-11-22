#include "Brain.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"
#include <fstream>
#include <sstream>
#include "../RobotState.h"
#include "../Config/DriverStationConfig.h"

#include "Autonomous.h"

#include "Events/GameModeChangeEvent.h"
#include "Events/JoystickMovedEvent.h"

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
}

Brain::Brain()
{
	m_driver_stick = new DebouncedJoystick(
			DriverStationConfig::JoystickConfig::DRIVER_STICK_PORT,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_BUTTONS,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_AXES);
	m_operator_stick = new DebouncedJoystick(
			DriverStationConfig::JoystickConfig::OPERATOR_STICK_PORT,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_BUTTONS,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_AXES);
	m_driver_wheel = new DebouncedJoystick(
			DriverStationConfig::JoystickConfig::DRIVER_WHEEL_PORT,
			DriverStationConfig::JoystickConfig::NUM_WHEEL_BUTTONS,
			DriverStationConfig::JoystickConfig::NUM_WHEEL_AXES);
	
	Autonomous *auton = new Autonomous();
	auton->AddStartEvent(new GameModeChangeEvent(RobotState::AUTONOMOUS));
	auton->AddAbortEvent(new JoystickMovedEvent(m_driver_stick));
	m_automation.push_back(auton);
}

Brain::~Brain()
{
	for (vector<Automation*>::iterator it = m_automation.begin(); it < m_automation.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	m_automation.clear();
}

void Brain::Update()
{
	for (vector<Automation*>::iterator it = m_automation.begin(); it < m_automation.end(); it++)
	{
		(*it)->Update();
	}
}
