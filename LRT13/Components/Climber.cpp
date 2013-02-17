#include "Climber.h"
#include "../ComponentData/ClimberData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

using namespace data::climber;
using namespace data;
using namespace drivetrain;

Climber::Climber() :
	Component("Climber", DriverStationConfig::DigitalIns::CLIMBER, true), m_configSection("climber"),
			m_winch_worm(RobotConfig::CAN::WINCH_WORM, "WinchWorm"),
			m_digital_input_left(RobotConfig::Digital::PTO_SWITCH_LEFT),
			m_digital_input_right(RobotConfig::Digital::PTO_SWITCH_RIGHT),
			m_servo_left(RobotConfig::PWM::LEFT_PTO_SERVO, "leftServo"),
			m_servo_right(RobotConfig::PWM::RIGHT_PTO_SERVO, "rightServo"),
			m_winch_gear_tooth((UINT32) RobotConfig::Digital::WINCH_GEAR_TOOTH)
{
	m_pneumatics = Pneumatics::Instance();
	m_state = IDLE;
	m_driving_encoders = DriveEncoders::GetInstance();
	m_climbing_level = 0;
	m_paused = false;
	m_previous_state = IDLE;
}

Climber::~Climber()
{

}

void Climber::onEnable()
{

}

void Climber::onDisable()
{
}

void Climber::enabledPeriodic()
{
	if (m_paused)
	{
		if (m_componentData->climberData->shouldContinueClimbing())
			m_paused = false;
		else
			return;
	}
	switch (m_state)
	{
	case IDLE:
		if (m_componentData->climberData->getDesiredClimbingStep() == INTENDED_ARM_UP)
		{
			m_state = ARM_UP_INITIAL;
		}
		if (m_climbing_level < 1)
		{
			m_servo_left.SetMicroseconds(m_servo_left_disengaged_position);
			m_servo_right.SetMicroseconds(m_servo_right_disengaged_position);
		}
		break;
	case ARM_UP_INITIAL:
		m_pneumatics->setClimberArm(true);
		break;
	case WAIT:
		if (m_componentData->climberData->getDesiredClimbingStep() == INTENDED_CLIMBING)
		{
			m_state = ARM_DOWN;
			m_winch_worm.setCollectionFlags(AsyncCANJaguar::OUTCURR);
		}
		break;
	case ARM_DOWN:
		m_pneumatics->setClimberArm(false);
		m_winch_worm.SetDutyCycle(-1.0);
		if (m_winch_worm.GetOutputCurrent() > m_winch_current_threshold)
		{
			if (m_climbing_level < 1)
				m_winch_worm.SetDutyCycle(m_winch_engage_duty_cycle);
			else
				m_winch_worm.SetDutyCycle(0.0);
			m_state = ENGAGE_PTO;
			m_hasStartedEngaging = false;
		}
		break;
	case ENGAGE_PTO:
		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
		if (m_climbing_level < 1)
			m_winch_worm.SetDutyCycle(m_winch_engage_duty_cycle);
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
			m_state = WINCH_UP;
			m_drive_train_position = m_driving_encoders->getRobotDist(); //awk units (2/3 in)
		}
		else
		{
			m_hasStartedEngaging = false;
			m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
			m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		}
		m_servo_left.SetMicroseconds(m_servo_left_engaged_position);
		m_servo_right.SetMicroseconds(m_servo_right_engaged_position);

		break;
	case WINCH_UP:
		m_timer = 0;
		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 1.0);
		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		if (fabs(m_driving_encoders->getRobotDist() - m_drive_train_position) > m_drive_train_position_threshold)
		{
			m_state = ENGAGE_HOOKS;
			m_climbing_level++;
		}
		break;
	case ENGAGE_HOOKS:
		m_pneumatics->setHookPosition(true);
		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
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

	m_previous_state = m_state;
}

void Climber::disabledPeriodic()
{
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
