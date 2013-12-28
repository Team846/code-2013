#include "DrivetrainInputs.h"
#include "../../Config/RobotConfig.h"

DrivetrainInputs::DrivetrainInputs()
{
	m_driver_stick = LRTDriverStation::Instance()->GetDriverStick();
	m_driver_wheel = LRTDriverStation::Instance()->GetDriverWheel();
	
	drivetrainData = DrivetrainData::Get();
	RegisterResource(DRIVE);
	RegisterResource(TURN);
	
	lastStop = false;
	driveSign = -1;
}

void DrivetrainInputs::Update()
{
	drivetrainData->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::VELOCITY_CONTROL);
	drivetrainData->SetControlMode(DrivetrainData::TURN, DrivetrainData::VELOCITY_CONTROL);
	
	double turn = 0.0;
	turn = -m_driver_wheel->GetAxis(Joystick::kXAxis);
	
	int sign = turn > 0 ? 1 : -1;
	
	turn = sign * pow(turn , 2);

	//			turn *= turn * sign;
	//turn = -m_driver_stick->GetAxis(Joystick::kZAxis);

	double forward = pow(
			driveSign * m_driver_stick->GetAxis(Joystick::kYAxis),
			RobotConfig::Drive::THROTTLE_EXPONENT);

	int signForward = forward > 0 ? 1 : -1;

	if (fabs(forward) < RobotConfig::Drive::DEADBAND)
		forward = 0.0;
	else
	{
		forward -= signForward * RobotConfig::Drive::DEADBAND;
		forward /= 1.0 - RobotConfig::Drive::DEADBAND;
	}
	

	//blending routine
	double absForward = fabs(forward); //to ensure correct arc when switching direction

	double blend = pow((1 - absForward), RobotConfig::Drive::BLEND_EXPONENT); //always between 0 and 1, raised to an exponent to adjust transition between in place and arc.

	const double turnInPlace = turn; //normal turn
	const double constRadiusTurn = turn * absForward; //arc turn

	double turnComposite = turnInPlace * (blend) + constRadiusTurn
			* (1 - blend); //blended function

	if (m_driver_wheel->IsButtonDown(
			DriverStationConfig::JoystickButtons::STOP_ROBOT))
	{
		forward = 0.0;
		turnComposite = 0.0;
		turn = 0.0;
//		if (!lastStop)
//		{
//			stoppedForward
//					= drivetrainData->getCurrentPos(
//							FORWARD);
//			stoppedTurn
//					= drivetrainData->getCurrentPos(
//							TURN);
//		}
//		drivetrainData->setControlMode(FORWARD,
//				POSITION_CONTROL);
//		drivetrainData->setRelativePositionSetpoint(
//				FORWARD,
//				stoppedForward
//						- drivetrainData->getCurrentPos(
//								FORWARD), 1.0);
//
//		drivetrainData->setControlMode(TURN,
//				POSITION_CONTROL);
//		drivetrainData->setRelativePositionSetpoint(
//				TURN,
//				stoppedTurn
//						- drivetrainData->getCurrentPos(
//								TURN), 1.0);
		lastStop = true;
	}
	else
		lastStop = false;
	if (m_driver_wheel->IsButtonJustPressed(
		DriverStationConfig::JoystickButtons::REVERSE_DRIVE))
	{
		driveSign = -driveSign;
	}

	drivetrainData->SetVelocitySetpoint(DrivetrainData::FORWARD, forward);
	drivetrainData->SetVelocitySetpoint(DrivetrainData::TURN, turnComposite);
}
