#include "TeleopInputs.h"
#include "DebouncedJoystick.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/DriverStationConfig.h"

using namespace data;

TeleopInputs::TeleopInputs(char * taskName, INT32 priority) :
	AsyncProcess(taskName, priority)
{
	m_actionData = ComponentData::GetInstance();
	m_driver_stick = new DebouncedJoystick(DriverStationConfig::JoystickConfig::DRIVER_STICK_PORT,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_BUTTONS,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_AXES);
	m_operator_stick = new DebouncedJoystick(DriverStationConfig::JoystickConfig::OPERATOR_STICK_PORT,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_BUTTONS,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_AXES);
	m_driver_wheel = new DebouncedJoystick(DriverStationConfig::JoystickConfig::DRIVER_WHEEL_PORT,
			DriverStationConfig::JoystickConfig::NUM_WHEEL_BUTTONS,
			DriverStationConfig::JoystickConfig::NUM_WHEEL_AXES);
}

TeleopInputs::~TeleopInputs()
{

}

INT32 TeleopInputs::Tick()
{
	m_driver_stick->Update();
	m_operator_stick->Update();
	
	Update();
	
	return 0;
}

void TeleopInputs::Update()
{
	RobotData::RobotState current_state = RobotData::GetCurrentState();
	
	if(current_state == RobotData::TELEOP)
	{
		if(m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::RESET_ZERO))
		{
			m_actionData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		}
		else
		{
			
		}
	}
	
	if(m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::SAVE_CONFIG))
		m_actionData->configLoaderData->save = true;
	if(m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::LOAD_CONFIG))
		m_actionData->configLoaderData->load= true;
	if(m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::APPLY_CONFIG))
		m_actionData->configLoaderData->apply = true;
}
