#include "TeleopInputs.h"
#include "DebouncedJoystick.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/DriverStationConfig.h"
#include "../Config/RobotConfig.h"
#include <cmath>

using namespace data;

TeleopInputs::TeleopInputs(char * taskName, INT32 priority) :
	AsyncProcess(taskName, priority)
{
	m_componentData = ComponentData::GetInstance();
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
	
	/************************Drivetrain************************/
	
	m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
	m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
	if(current_state == RobotData::TELEOP)
	{
		if(m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::RESET_ZERO))
		{
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		}
		else
		{
			double turn = 0.0;
			turn = -m_driver_wheel->GetAxis(Joystick::kXAxis);
			turn = pow(turn, RobotConfig::drive::BLEND_EXPONENT);

			double forward = pow(-m_driver_stick->GetAxis(Joystick::kYAxis), RobotConfig::drive::THROTTLE_EXPONENT);

			double turnComposite = 0.0;

			double absForward = fabs(forward);
			double blend = (1 - absForward);
			blend = pow(blend, 2);

			const double turnInPlace = turn;
			const double turnConstantRadius = turn * absForward;
			turnComposite = turnInPlace * (blend) + turnConstantRadius * (1
					- blend);

			m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, forward);
			m_componentData->drivetrainData->setVelocitySetpoint(TURN, forward);
		}
	}
	
	/************************Config************************/
	
	if(m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::SAVE_CONFIG))
		m_componentData->configLoaderData->save = true;
	if(m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::LOAD_CONFIG))
		m_componentData->configLoaderData->load= true;
	if(m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::APPLY_CONFIG))
		m_componentData->configLoaderData->apply = true;
}
