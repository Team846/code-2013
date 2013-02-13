#include "TeleopInputs.h"
#include "DebouncedJoystick.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/DriverStationConfig.h"
#include "../Config/RobotConfig.h"
#include "../Utils/AsyncPrinter.h"
#include "AutoActions.h"
#include <cmath>

using namespace data;

TeleopInputs::TeleopInputs(char * taskName, INT32 priority) :
	SynchronizedProcess(taskName, priority)
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
	
	m_autoActions = new AutoActions();
}

TeleopInputs::~TeleopInputs()
{

}

INT32 TeleopInputs::Tick()
{
	m_driver_stick->Update();
	m_operator_stick->Update();
	m_driver_wheel->Update();
	Update();
	
	return 0;
}

void TeleopInputs::Update()
{
	RobotData::RobotState current_state = RobotData::GetCurrentState();
	
	/************************Drivetrain************************/
	
//#define USEOPENLOOP
	// Use velocity control in teleoperated mode
#ifdef USEOPENLOOP
	m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
	m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
#else
	m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
	m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
#endif
	if(current_state == RobotData::TELEOP)
	{
		if(m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::RESET_ZERO))
		{
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		}
		else
		{
			double turn = 0.0;
//			turn = -m_driver_wheel->GetAxis(Joystick::kXAxis);
			turn = -m_driver_stick->GetAxis(Joystick::kZAxis);

			double forward = pow(-m_driver_stick->GetAxis(Joystick::kYAxis), RobotConfig::Drive::THROTTLE_EXPONENT);
#ifdef USEOPENLOOP
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, forward);
			m_componentData->drivetrainData->setOpenLoopOutput(TURN, turn);
#else
			m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, forward);
			m_componentData->drivetrainData->setVelocitySetpoint(TURN, turn);
#endif
		}
	}
	
	/************************Automatic Functions************************/

	if(m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::AUTO_AIM))
	{
		m_autoActions->AutoAim();
	}
	else if (m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::AUTO_CLIMB))
	{
		m_autoActions->AutoClimb();
	}
	else if (m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::DISC_TRACK))
	{
		m_autoActions->DiscTrack();
	}
	
	/************************Config************************/
	
	if(m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::SAVE_CONFIG))
		m_componentData->configLoaderData->RequestSave();
	if(m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::LOAD_CONFIG))
		m_componentData->configLoaderData->RequestLoad();
	if(m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::APPLY_CONFIG))
		m_componentData->configLoaderData->RequestApply();
}
