#include "TeleopInputs.h"
#include "DebouncedJoystick.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/DriverStationConfig.h"
#include "../Config/RobotConfig.h"
#include "../Utils/AsyncPrinter.h"
#include "AutoActions.h"
#include "../ComponentData/ClimberData.h"
#include <cmath>

using namespace data;
using namespace climber;

TeleopInputs::TeleopInputs(char * taskName, INT32 priority) :
	SynchronizedProcess(taskName, priority)
{
	m_componentData = ComponentData::GetInstance();
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

	m_autoActions = new AutoActions();
}

TeleopInputs::~TeleopInputs()
{
	DELETE(m_driver_stick);
	DELETE(m_operator_stick);
	DELETE(m_driver_wheel);
	
	DELETE(m_autoActions);
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

	#define USEOPENLOOP
	// Use velocity control in teleoperated mode
#ifdef USEOPENLOOP
	
	m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
	m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
#else
	m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
	m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
#endif
	if (current_state == RobotData::TELEOP)
	{
		if (m_driver_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::RESET_ZERO))
		{
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		}
		else
		{
			double turn = 0.0;
			//			turn = -m_driver_wheel->GetAxis(Joystick::kXAxis);
			turn = -m_driver_stick->GetAxis(Joystick::kZAxis);

			double forward = pow(-m_driver_stick->GetAxis(Joystick::kYAxis),
					RobotConfig::Drive::THROTTLE_EXPONENT);
			
			//blending routine
			double absForward = fabs(forward); //to ensure correct arc when switching direction

			double blend = pow((1 - absForward),
					RobotConfig::Drive::BLEND_EXPONENT); //always between 0 and 1, raised to an exponent to adjust transition between in place and arc.

			const double turnInPlace = turn; //normal turn
			const double constRadiusTurn = turn * absForward; //arc turn

			double turnComposite = turnInPlace * (blend) + constRadiusTurn
					* (1 - blend); //blended function
			
#ifdef USEOPENLOOP
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, forward);
			m_componentData->drivetrainData->setOpenLoopOutput(TURN, turnComposite);
#else
			m_componentData->drivetrainData->setVelocitySetpoint(FORWARD,
					forward);
			m_componentData->drivetrainData->setVelocitySetpoint(TURN, turn);
#endif
		}
	}

	/************************Climber Functions************************/
	if (m_driver_stick->IsButtonJustPressed(
			DriverStationConfig::JoystickButtons::AUTO_AIM_PYRAMID))
	{
		switch (m_componentData->climberData->getDesiredClimbingStep())
		{
		case INTENDED_IDLE:
			m_componentData->climberData->setDesiredClimbingStep(
					INTENDED_ARM_UP);
			break;
		case INTENDED_ARM_UP:
			m_componentData->climberData->setDesiredClimbingStep(
					INTENDED_CLIMBING);
			break;
		case INTENDED_CLIMBING:
			break;
		}
	}
	m_componentData->climberData->setShouldContinueClimbing(
			m_driver_stick->IsButtonJustPressed(DriverStationConfig::JoystickButtons::CONTINUE_CLIMB));

	/************************Automatic Functions************************/

	if (m_driver_stick->IsButtonDown(
			DriverStationConfig::JoystickButtons::AUTO_AIM_BACKBOARD))
	{
		m_autoActions->EnableAutoAimBackboard();
	}
	else if (m_driver_stick->IsButtonDown(
			DriverStationConfig::JoystickButtons::AUTO_AIM_PYRAMID))
	{
		m_autoActions->EnableAutoAimPyramid();
	}
	else if (m_driver_stick->IsButtonDown(
			DriverStationConfig::JoystickButtons::FEEDER_STATION_APPROACH))
	{
		m_autoActions->EnableFeederStationTrack();
	}
	else
	{
		m_autoActions->Reset();
	}

	/************************Config************************/

	if (m_driver_stick->IsButtonJustPressed(
			DriverStationConfig::JoystickButtons::SAVE_CONFIG))
		m_componentData->configLoaderData->RequestSave();
	if (m_driver_stick->IsButtonJustPressed(
			DriverStationConfig::JoystickButtons::LOAD_CONFIG))
		m_componentData->configLoaderData->RequestLoad();
	if (m_driver_stick->IsButtonJustPressed(
			DriverStationConfig::JoystickButtons::APPLY_CONFIG))
		m_componentData->configLoaderData->RequestApply();
}
