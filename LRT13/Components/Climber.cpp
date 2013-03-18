#include "Climber.h"
#include "../ComponentData/ClimberData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../ComponentData/ShooterData.h"
#include "../Config/DriverStationConfig.h"

using namespace data::climber;
using namespace data;
using namespace shooter;
using namespace drivetrain;

Climber::Climber() :
	Component("Climber", DriverStationConfig::DigitalIns::CLIMBER, true), m_configSection("Climber"),
			m_winch_worm(RobotConfig::CAN::WINCH_WORM, "WinchWorm"),
			m_digital_input_left(RobotConfig::Digital::PTO_SWITCH_LEFT),
			m_digital_input_right(RobotConfig::Digital::PTO_SWITCH_RIGHT),
			m_servo_left(RobotConfig::Servo::LEFT_PTO_SERVO, "leftServo"),
			m_servo_right(RobotConfig::Servo::RIGHT_PTO_SERVO, "rightServo"),
			m_winch_gear_tooth((UINT32) RobotConfig::Digital::WINCH_GEAR_TOOTH)
{
	m_pneumatics = Pneumatics::Instance();
	m_state = IDLE;
	m_driving_encoders = DriveEncoders::GetInstance();
	m_climbing_level = 0;
	m_paused = false;
	m_previous_state = IDLE;
	m_winch_gear_tooth.Start();
}

Climber::~Climber()
{

}

void Climber::onEnable()
{
}

void Climber::onDisable()
{
	disabledPeriodic();
//	static int e = 0;
//	e++;
//		if (e % 10 == 0)
//			AsyncPrinter::Printf("Climber Disabled\n");
//	m_servo_left.SetEnabled(true);
//	m_servo_right.SetEnabled(true);
//	m_servo_left.SetMicroseconds(m_servo_left_disengaged_position);
//	m_servo_right.SetMicroseconds(m_servo_right_disengaged_position);
}	

void Climber::enabledPeriodic()
{
	static bool wasDebugging = false;
	if (m_componentData->climberData->shouldDebug())
	{
//		AsyncPrinter::Printf("Debug\n");
		if (m_componentData->climberData->shouldChangeAngleState())
		{
			if (m_componentData->shooterData->ShouldLauncherBeHigh())
				m_componentData->shooterData->SetLauncherAngleLow();
			else
				m_componentData->shooterData->SetLauncherAngleHigh();
		}
		
		if (m_componentData->climberData->shouldChangeArmState())
		{
//			AsyncPrinter::Printf("Arm for realz\n");
			m_pneumatics->setClimberArm(!m_pneumatics->GetClimberState());
		}
		
		if (m_componentData->climberData->shouldWinchPawlGoDown())
			m_winch_worm.SetDutyCycle(0.4);
		else if (m_componentData->climberData->shouldWinchPawlGoUp())
			m_winch_worm.SetDutyCycle(-0.4);
		else
		{
//			AsyncPrinter::Printf("You failed a tlife!\n");
			m_winch_worm.SetDutyCycle(0.0);
		}
		
		if (m_componentData->climberData->shouldPTOChangeDisengage())
		{
			m_servo_left.SetMicroseconds(m_servo_left_disengaged_position);
			m_servo_right.SetMicroseconds(m_servo_right_disengaged_position);
		}
		else if (m_componentData->climberData->shouldPTOChangeEngage())
		{
			m_servo_left.SetMicroseconds(m_servo_left_engaged_position);
			m_servo_right.SetMicroseconds(m_servo_right_engaged_position);
		}
//		AsyncPrinter::Printf("Still alive %d\n", GetFPGATime());
		return;
	}
//	AsyncPrinter::Printf("WTF\n");
	
	static int e = 0;
	e++;
	m_servo_left.SetEnabled(true);
	m_servo_right.SetEnabled(true);
	if (e % 10 == 0)
	switch(m_state)
	{
	case IDLE:
		AsyncPrinter::Printf("Idle\n");
		break;
	case ARM_UP_INITIAL:
		AsyncPrinter::Printf("Arm up\n");
		break;
	case WAIT:
		AsyncPrinter::Printf("Wait\n");
		break;
	case ARM_DOWN:
		AsyncPrinter::Printf("Arm Down\n");
		break;
	case DUMB_ENGAGE_PTO:
		AsyncPrinter::Printf("Dumb Engage PTO\n");
		break;
	case ENGAGE_PTO:
		AsyncPrinter::Printf("Engage PTO\n");
		AsyncPrinter::Printf("Left %d, rigt %d\n", m_digital_input_left.Get(), m_digital_input_right.Get());
		break;
	case WINCH_UP:
		AsyncPrinter::Printf("WINCH_UP\n");
		break;
	case ENGAGE_HOOKS:
		AsyncPrinter::Printf("ENGAGE HOOKS\n");
		break;
	case DISENGAGE_PTO:
		m_state = ARM_UP_FINAL;
		break;
	case ARM_UP_FINAL:
		AsyncPrinter::Printf("ARM_UP_FINAl\n");
		m_state = WAIT;
		break;
	}
	
	
	if (m_paused)
	{
		if (e % 10 == 0)
			AsyncPrinter::Printf("Paused\n");
		if (m_componentData->climberData->shouldContinueClimbing())
		{
			m_paused = false;
			AsyncPrinter::Printf("Unpausing\n");
		}
		else
			return;
	}
	
	static double driveSpeed = 0.0;
	double curr;
	switch (m_state)
	{
	case IDLE:
		m_pneumatics->setClimberArm(RETRACTED, true);
		m_winch_worm.SetDutyCycle(0.0);
		if (m_componentData->climberData->getDesiredClimbingStep() == INTENDED_ARM_UP)
		{
			m_state = ARM_UP_INITIAL;
//			m_state = ENGAGE_PTO;
		}
		if (m_climbing_level < 1)
		{
			m_servo_left.SetMicroseconds(m_servo_left_disengaged_position);
			m_servo_right.SetMicroseconds(m_servo_right_disengaged_position);
//			AsyncPrinter::Printf("Disengaging\n");
		}
		else
		{
//			AsyncPrinter::Printf("Not Disengaging\n");
		}
		break;
	case ARM_UP_INITIAL:
		m_componentData->shooterData->SetLauncherAngleLow();
		m_pneumatics->setClimberArm(EXTENDED);
		m_state = WAIT;
		break;
	case WAIT:
//		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
//		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
////		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 1.0);
//		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.5);
//		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if (m_componentData->climberData->getDesiredClimbingStep() == INTENDED_CLIMBING)
		{
			m_state = ARM_DOWN;
			m_winch_worm.setCollectionFlags(AsyncCANJaguar::OUTCURR);
		}
		break;
	case ARM_DOWN:
#define DIRECTION 1
		m_pneumatics->setClimberArm(EXTENDED);
		m_componentData->shooterData->SetLauncherAngleLow();
		m_winch_worm.SetDutyCycle(-1.0 * DIRECTION);
		curr = m_winch_worm.GetOutputCurrent() ;
		if (curr > m_winch_current_threshold)
		{
			m_timer--;
			if (m_timer < 0)
			{
				if (m_climbing_level < 1)
					m_winch_worm.SetDutyCycle(-m_winch_engage_duty_cycle * DIRECTION);
				else
					m_winch_worm.SetDutyCycle(0.0);
				m_state = ENGAGE_PTO;
				m_hasStartedEngaging = false;
				m_timer = 25;
			}
		}
		else
		{
			AsyncPrinter::Printf("Curr %.2f\n", curr);
			m_timer = 10;
		}
		break;
		//TODO add a case that moves arm back and picks up slop
	case PREPARE_CLIMBING_POSITION:
//		m_winch_worm.SetDutyCycle(1.0);
//		m_pneumatics->setClimberArm(RETRACTED);
//		m_componentData->shooterData->SetLauncherAngleLow();
//		m_timer--;
//		if (m_timer < 0)
//		{
//			m_winch_worm.SetDutyCycle()
//		}
		break;
	case ENGAGE_PTO:
//		m_pneumatics->setClimberArm(false);
		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
		if (m_climbing_level < 1)
			m_winch_worm.SetDutyCycle(-m_winch_engage_duty_cycle);
		else
			m_winch_worm.SetDutyCycle(0.0);

		if (m_digital_input_left.Get() ^ m_digital_input_right.Get()) // only one side engaged
		{
			if (m_digital_input_left.Get()) // left side engaged
				m_side_engaged = 1;
			else
				// right side engaged
				m_side_engaged = -1;

			if (m_winch_worm.GetOutputCurrent() > m_winch_current_threshold)
			{
				m_hasStartedEngaging = true;
			}
			else if (m_hasStartedEngaging)
			{
				m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
				m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.03 * m_side_engaged);
			}
		}
		else if (!m_digital_input_left.Get() && !m_digital_input_left.Get()) // both engaged
		{
			m_timer = 5;//for unlocking the pawl
			m_state = WINCH_UP;
			driveSpeed = 0.0;
			m_drive_train_position = m_driving_encoders->getRobotDist(); //awk units (2/3 in)
		}
		else
		{
			m_hasStartedEngaging = false;
			m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
			m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		}
		m_servo_left.SetEnabled(true);
		m_servo_right.SetEnabled(true);
		m_servo_left.SetMicroseconds(m_servo_left_engaged_position);
		m_servo_right.SetMicroseconds(m_servo_right_engaged_position);
		// TODO: disable servo afterwards
		break;
	case UNLOCK_PAWL:
		m_timer--;
		if (m_timer < 0)
		{
			m_state = WINCH_UP;
			driveSpeed = 0.0;
		}
		else
		{
			m_winch_worm.SetDutyCycle(1.0 * DIRECTION);
		}
		break;
	case WINCH_UP:
		m_timer = 0;
//		m_pneumatics->setClimberArm(RETRACTED);
//		m_componentData->shooterData->SetLauncherAngleLow();
		m_winch_worm.SetDutyCycle(-1.0 * DIRECTION);
		//30:1 on worm, 4:1, 19k rpm, 158.33333333333333333333333333333 max
		//11:1, 14-54, 5400/ 42.428571428571428571428571428571  = 130
		if (fabs(driveSpeed) < 0.99)
		{
			driveSpeed += 0.02 * DIRECTION;
		}
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
//		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 1.0 * DIRECTION);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, driveSpeed);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if (fabs(m_driving_encoders->getRobotDist() - m_drive_train_position) > m_drive_train_position_threshold)//SHould be about 42
		{
			m_winch_worm.SetDutyCycle(0.0);
			m_state = ENGAGE_HOOKS;
			m_climbing_level++;
			m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
			m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
			m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		}
		else
		{
			AsyncPrinter::Printf("dist: %.2f\n", fabs(m_driving_encoders->getRobotDist() - m_drive_train_position) );
		}
		break;
	case ENGAGE_HOOKS:
		m_pneumatics->setHookPosition(EXTENDED);
//		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
//		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
//		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
//		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
//		return;
		if (++m_timer > m_timer_threshold)
		{
			if (m_climbing_level < 3)
			{
				m_state = DISENGAGE_PTO;
				m_timer = 0;
			}
			else
			{
				m_state = IDLE;
			}

			m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
			m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
			m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		}
		break;
	case DISENGAGE_PTO:
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		m_servo_left.SetEnabled(true);
		m_servo_right.SetEnabled(true);
		m_servo_left.SetMicroseconds(m_servo_left_disengaged_position);
		m_servo_right.SetMicroseconds(m_servo_right_disengaged_position);
		if (++m_timer > m_disengageTimer_threshold)
		{
			m_state = ARM_UP_FINAL;
			m_timer = 0;
			m_gear_tooth_ticks_position = m_winch_gear_tooth.Get();
		}
		break;
	case ARM_UP_FINAL:
		m_pneumatics->setClimberArm(true);
		m_winch_worm.SetDutyCycle(1.0);
		if (m_winch_gear_tooth.Get() - m_gear_tooth_ticks_position > m_gear_tooth_threshold)
		{
			m_winch_worm.SetDutyCycle(0.0);
			m_state = ARM_DOWN;
		}
		break;
	}
	if (m_previous_state != m_state)
	{
		m_paused = true; //we pause
	}
	
	if (m_componentData->climberData->shouldForceContinueClimbing())
	{
		switch(m_state)
		{
		case IDLE:
			m_state = ARM_UP_INITIAL;
			break;
		case ARM_UP_INITIAL:
			m_state = WAIT;
			break;
		case WAIT:
			m_state = ARM_DOWN;
			break;
		case ARM_DOWN:
			m_state = DUMB_ENGAGE_PTO ;//TODO notice this, change to real engage PTO
			break;
		case DUMB_ENGAGE_PTO:
			m_state = WINCH_UP;
			break;
		case ENGAGE_PTO:
			m_state = WINCH_UP;
			break;
		case WINCH_UP:
			m_state = ENGAGE_HOOKS;
			break;
		case ENGAGE_HOOKS:
			m_state = DISENGAGE_PTO;
			break;
		case DISENGAGE_PTO:
			m_state = ARM_UP_FINAL;
			break;
		case ARM_UP_FINAL:
			m_state = WAIT;
			break;
		}
	}

	m_previous_state = m_state;
}

void Climber::disabledPeriodic()
{
	m_componentData->climberData->setDesiredClimbingStep(INTENDED_IDLE );
	m_state = IDLE;//Restart the routine, makes debugging easier.
	static int e = 0;
	e++;
//	if (e % 10 == 0)
//		AsyncPrinter::Printf("Climber Disabled\n");
	m_servo_left.SetEnabled(true);
	m_servo_right.SetEnabled(true);
	m_servo_left.SetMicroseconds(m_servo_left_disengaged_position);
	m_servo_right.SetMicroseconds(m_servo_right_disengaged_position);
}

void Climber::Configure()
{
	m_winch_current_threshold = m_config->Get<double> (m_configSection, "winchCurrentThreshold", 15.0);
	m_winch_engage_duty_cycle = m_config->Get<double> (m_configSection, "winchEngageDutyCycle", 0.05);
	m_timer_threshold = m_config->Get<int> (m_configSection, "timerThreshold", 25);
	m_drive_train_position_threshold = m_config->Get<double> (m_configSection, "driveTrainPositionThreshold", 42);
	m_servo_left_engaged_position = m_config->Get<int> (m_configSection, "leftServoEngaged", 1514);
	m_servo_right_engaged_position = m_config->Get<int> (m_configSection, "rightServoEngaged", 1514);
	m_servo_left_disengaged_position = m_config->Get<int> (m_configSection, "leftServoDisengaged", 905);
	m_servo_right_disengaged_position = m_config->Get<int> (m_configSection, "rightServoDisengaged", 905);
}

void Climber::Log()
{

}
