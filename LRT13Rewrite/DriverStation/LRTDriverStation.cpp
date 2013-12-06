#include "LRTDriverStation.h"
#include "../RobotState.h"

LRTDriverStation* LRTDriverStation::m_instance = NULL;

void LRTDriverStation::Initialize()
{
	if (m_instance == NULL)
		m_instance = new LRTDriverStation();
}

LRTDriverStation* LRTDriverStation::Instance()
{
	if (m_instance == NULL)
		m_instance = new LRTDriverStation();
	return m_instance;
}

void LRTDriverStation::Finalize()
{
    delete m_instance;
    m_instance = NULL;
}

LRTDriverStation::LRTDriverStation()
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
}

LRTDriverStation::~LRTDriverStation()
{
	delete m_driver_stick;
	m_driver_stick = NULL;
	delete m_operator_stick;
	m_operator_stick = NULL;
	delete m_driver_wheel;
	m_driver_wheel = NULL;
}

void LRTDriverStation::Update()
{
	if (RobotState::Instance().GameMode() != RobotState::DISABLED)
	{
		Instance()->m_driver_stick->Update();
		Instance()->m_operator_stick->Update();
		Instance()->m_driver_wheel->Update();
	}
}

DebouncedJoystick* LRTDriverStation::GetOperatorStick()
{
    return m_operator_stick;

}

DebouncedJoystick* LRTDriverStation::GetDriverStick()
{
    return m_driver_stick;

}

DebouncedJoystick* LRTDriverStation::GetDriverWheel()
{
    return m_driver_wheel;
}
