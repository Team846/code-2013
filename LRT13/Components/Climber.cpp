#include "Climber.h"
#include "../ComponentData/LEDIndicatorData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../ComponentData/CollectorData.h"
//#define STEP

using namespace data::climber;
using namespace data;
using namespace shooter;
using namespace drivetrain;

Climber::Climber() :
	Component("Climber", DriverStationConfig::DigitalIns::CLIMBER, true), m_configSection("Climber"),
			m_digital_input_left(RobotConfig::Digital::PTO_SWITCH_LEFT),
			m_digital_input_right(RobotConfig::Digital::PTO_SWITCH_RIGHT),
			m_servo_left(RobotConfig::Servo::LEFT_PTO_SERVO, "leftServo"),
			m_servo_right(RobotConfig::Servo::RIGHT_PTO_SERVO, "rightServo"),
			m_winch_gear_tooth((UINT32) RobotConfig::Digital::WINCH_GEAR_TOOTH),
			m_logFile()
{
	m_climberArms = new Pneumatics(RobotConfig::Solenoid::CLIMBER_A, RobotConfig::Solenoid::CLIMBER_B, "ClimberArms");
	m_hooks = new Pneumatics(RobotConfig::Solenoid::HOOKS, "Hooks");
	m_pressure_a = new AnalogChannel(RobotConfig::Analog::PRESSURE_A);
	m_pressure_b = new AnalogChannel(RobotConfig::Analog::PRESSURE_B);
	m_state = NOTHING;
	m_driving_encoders = DriveEncoders::GetInstance();
	m_paused = false;
	m_previous_state = NOTHING;
	m_winch_gear_tooth.Start();
	
	m_winchPawl = m_componentData->winchPawlData;
	m_climberData = m_componentData->climberData;
	m_shooterData = m_componentData->shooterData;
	
	m_driveSpeed = 0.0;
	
	m_ptoEngaged = false;
	m_pressure_scale = 1400;
	table = NetworkTable::GetTable("RobotData");
}

Climber::~Climber()
{

}

void Climber::onEnable()
{
	if (!m_logFile.is_open())
		m_logFile.open("/climber.log");
}

void Climber::onDisable()
{
	if(m_logFile.is_open())
		m_logFile.close();
	
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
	//Profiler::BeginActivity("Climber::enabledPeriodic()");
	m_logFile << "Time: " << Timer::GetFPGATimestamp() << endl;
	
	const double curr = m_winchPawl->getMotorCurrent();
	
	m_componentData->climberData->setWinchPawlCurrent(curr);
	
	m_paused = false; //we pause

//	if(m_state != INACTIVE && m_winchPawl->isWinchPawlTimedOut())
//	{
//		m_climberData->setShouldPotentiallyAbort(true);
//	}
	
	if (m_state != NOTHING && m_componentData->climberData->shouldPotentiallyAbort())
	{
		m_climberData->setShouldPotentiallyAbort(false);
		m_state = NOTHING;
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		
		disengagePTO();
		
		winchPawlOff();
		
		m_paused = false;
	}
	
	if (m_componentData->climberData->shouldDebug())
	{
		m_componentData->climberData->disableDebug();
		
		m_climberData->setCurrentState(DEBUG_MODE);
		AsyncPrinter::Printf("Debug\n");
		
		if (m_componentData->climberData->shouldWinchPawlGoUp())
		{
			AsyncPrinter::Printf("winchpawl up %d\n", m_winch_gear_tooth.Get());
			winchPawlUp(false);
		}
		else if (m_componentData->climberData->shouldWinchPawlGoDown())
		{
			AsyncPrinter::Printf("winch pawl down %d\n", m_winch_gear_tooth.Get());
			winchPawlDown(true);
		}
		else
		{
			winchPawlOff();
		}
		if (m_componentData->climberData->shouldPTOChangeEngage())
		{
			AsyncPrinter::Printf("Engaging\n");
			engagePTO(true);
		}
		else if (m_componentData->climberData->shouldPTOChangeDisengage())
		{
			AsyncPrinter::Printf("Disengaging\n");
			disengagePTO(true);
		}
		if (m_componentData->climberData->shouldArmsChange())
		{
			if(m_componentData->climberData->shouldArmsUp())
			{
				m_climberArms->Set(Pneumatics::FORWARD, true);
			}
			else
			{
				m_climberArms->Set(Pneumatics::REVERSE, true);
			}
		}
		
		if (m_componentData->climberData->shouldHooksChange())
		{
			AsyncPrinter::Printf("Hooks change from %s\n", m_hooks->Get() == Pneumatics::OFF ? "OFF" : "ON");
			m_hooks->Set(m_hooks->Get() == Pneumatics::OFF ? Pneumatics::FORWARD : Pneumatics::OFF, true);
		}
		return;
	}

	state desiredStep = m_climberData->getDesiredState();
	
	if(desiredStep > NOTHING)
	{
//		m_state = WAIT;
		m_waitGoToState = desiredStep;
		m_climberData->setWaitingState(m_waitGoToState);
	}
	
	m_climberData->setWaitingState((climber::state)((m_state + 1) % 10 + 10)); // for access from TeleopInputs to scroll starting from current state
	m_climberData->setCurrentState(m_state);
	m_climberData->setDesiredState(NOTHING); // reset back to nothing

//	AsyncPrinter::Printf("WTF\n");
	
	static int e = 0;
	e++;
	m_servo_left.SetEnabled(true);
	m_servo_right.SetEnabled(true);
	//start at 24
	//mid at 42, max at
	
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
	
	string m_stateString = "???";
	
	if (m_state != NOTHING && m_state != LINE_UP)
	{
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);

	}
	
	switch(m_state)
	{
	case NOTHING:
		m_stateString = "NOTHING";
		
//		disengagePTO(true);
		
		winchPawlOff();
		
//		m_hooks->Set(Pneumatics::OFF, true);
//		m_climberArms->Set(Pneumatics::REVERSE, true);

		m_driveSpeed = 0.0;
		m_drive_train_position = m_winch_gear_tooth.Get();
		
		if(m_climberData->shouldContinueClimbing())
			m_state = BEGIN;
		break;
	case BEGIN:
		m_stateString = "BEGIN";
		m_hooks->Set(Pneumatics::FORWARD, true);
		m_climberArms->Set(Pneumatics::FORWARD, true);
		
		disengagePTO(true);
		
//		m_timer = m_componentData->collectorData->GetExtendTime() + 40;
		
		m_componentData->collectorData->SlideUp();
//		m_pneumatics->setCollector(RETRACTED, true);
		
		winchPawlUp();
		
		m_logFile << "arm up gear tooth: " << m_winch_gear_tooth.Get() << endl;
		AsyncPrinter::Printf("arm up Start pos: %d, Gear tooth: %d\n", m_drive_train_position, m_winch_gear_tooth.Get());
		
		if (abs(m_winch_gear_tooth.Get() - m_drive_train_position) > m_armUpTicks)
		{
			m_logFile << "stopping" << endl;
			
			winchPawlOff(); // all right, paul, you can take a breather now
			m_state = LINE_UP;
		}
		break;
	case LINE_UP:
		m_stateString = "LINE_UP";

		winchPawlOff();
		
		// Line up to the first bar
		
		if(m_climberData->shouldContinueClimbing())
		{
			m_state = HOOKS_DOWN;
		}
		break;
	case HOOKS_DOWN:
		m_stateString = "HOOKS_DOWN";
		m_hooks->Set(Pneumatics::OFF, true);
		
		m_shooterData->SetLauncherAngleHigh();
		
		m_state = ARMS_DOWN;
		break;
	case ARMS_DOWN:
		m_stateString = "ARMS_DOWN";

		winchPawlOff();
		
		// Engage arms to second bar
		
		if(m_climberData->shouldContinueClimbing())
		{
			m_state = CLIMB_PREPARE;
		}
		break;
//	case ARM_DOWN_PREPARE:
//		m_stateString = "ARM_DOWN_PREPARE";
//		
//		m_timer = 10;
//		if(m_climberData->shouldContinueClimbing())
//			m_state = CLIMB_PREPARE; // ARM_DOWN;
//		break;
//	case ARM_DOWN:
//		m_stateString = "ARM_DOWN";
//		
//		
//		// m_timer is set to 10 in ARM_DOWN_PREPARE
//		winchPawlDown();
//		
//		if(curr > m_winch_current_threshold)
//			--m_timer;
//		
//		if(m_timer <= 0)
//		{
////			if (m_climbing_level == GROUND)
////				m_winchPawl->setDutyCyle(m_winch_engage_duty_cycle * m_winchPawlDownDirection);
////			else
//				winchPawlOff();
//#ifdef STEP
//		if(m_climberData->shouldContinueClimbing())
//#endif
//			m_state = CLIMB_PREPARE;
//		}
//		break;
	case CLIMB_PREPARE:
		m_stateString = "CLIMB_PREPARE";
		
		m_hooks->Set(Pneumatics::OFF, true);

		engagePTO(true);
		
		m_drive_train_position = m_winch_gear_tooth.Get();
		
		m_logFile << "m_drive_train_position: " << m_drive_train_position << endl;
		
		m_driveSpeed = 0.0;

#ifdef STEP
		if(m_climberData->shouldContinueClimbing())
#endif
		m_state = CLIMB;
		
		break;
	case CLIMB:
		m_stateString = "CLIMB";
		
		// m_driveSpeed is set to 0.0 in CLIMB_PREPARE
		
		m_winchPawl->setDutyCyle(1.0F * m_winchPawlDownDirection);
		
		if (fabs(m_driveSpeed) < 1.0)
		{
			m_driveSpeed += 0.05 * m_winchPawlUpDirection; // drivetrain goes in the opposite direction of the pawl
			
			m_logFile << "m_driveSpeed: " << m_driveSpeed << endl;
		}
		
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, m_driveSpeed);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		m_logFile << "gear tooth: " << m_winch_gear_tooth.Get() << endl;
		AsyncPrinter::Printf("Start pos: %d, Gear tooth: %d\n", m_drive_train_position,m_winch_gear_tooth.Get());
		
		if (m_winch_gear_tooth.Get() - m_drive_train_position > m_climbingLevelGearToothTicks[1])//Should be about 42
		{
			m_logFile << "stopping" << endl;
			
			winchPawlOff();
		}
		break;
	}
	
	static int printCounter = 0;
	
	if(printCounter++ % 20 == 0 || m_state != m_previous_state)
		AsyncPrinter::Printf("Climb state: %s\n", m_stateString.c_str());
	LCD::Instance()->Print(4, 0, true, "Climb: %s", m_stateString.c_str());
	LCD::Instance()->Print(4, 0, true, "P: %.4f", (m_pressure_b->GetAverageVoltage() - m_pressure_a->GetAverageVoltage()) * m_pressure_scale);
	table->PutNumber("Pressure", (m_pressure_b->GetAverageVoltage() - m_pressure_a->GetAverageVoltage()) * m_pressure_scale);
	
	SmarterDashboard::Instance()->SetTelemetryData<string>(TelemetryType::CLIMBER_STATE, m_stateString);
	
	m_logFile << "State: " << m_stateString << endl;
	m_logFile << "Winch Pawl Current: " << m_winchPawl->getMotorCurrent() << " amps" << endl;
	m_logFile << "Winch Pawl Requested Out: " << m_winchPawl->getDutyCycle() << endl;
	m_logFile << "Drivetrain Forward Open Loop Out: " << m_componentData->drivetrainData->getOpenLoopOutput(FORWARD) << endl;
	m_logFile << endl;
	
	m_logFile.flush();

	m_previous_state = m_state;
}

void Climber::disengagePTO(bool force)
{
	if(!m_ptoEngaged && !force)
		return;
	
	m_servo_left.SetEnabled(true);
	m_servo_right.SetEnabled(true);
	m_servo_left.SetMicroseconds(m_servo_left_disengaged_position);
	m_servo_right.SetMicroseconds(m_servo_right_disengaged_position);
	
	m_ptoEngaged = false;
	
	m_logFile << "PTO disengaged." << endl;
}

void Climber::engagePTO(bool force)
{	
	if(m_ptoEngaged && !force)
		return;
	
	m_servo_left.SetEnabled(true);
	m_servo_right.SetEnabled(true);
	m_servo_left.SetMicroseconds(m_servo_left_engaged_position);
	m_servo_right.SetMicroseconds(m_servo_right_engaged_position);
	
	m_ptoEngaged = true;
	
	m_logFile << "PTO engaged." << endl;
}

void Climber::winchPawlUp(bool fast)
{
	double driveSpeed = 0.0;
	
	if(m_ptoEngaged)
	{
		fast = true; //not sketch at all
		driveSpeed = fast ? 1.0 : 0.3;
	}
	m_winchPawl->setDutyCyle((fast ? 1.0 : 0.5) * m_winchPawlUpDirection);


	
	m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
	m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
	m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, driveSpeed * m_winchPawlDownDirection);
	m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
}

void Climber::winchPawlDown(bool fast)
{
	double driveSpeed = 0.0;
	
	if(m_ptoEngaged)
	{
		fast = true;//not sketch at all
		driveSpeed = fast ? 1.0 : 0.3;
	}

	m_winchPawl->setDutyCyle((fast ? 1.0 : 0.5) * m_winchPawlDownDirection);
	
	m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
	m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
	m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, driveSpeed * m_winchPawlUpDirection);
	m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
}

void Climber::winchPawlOff()
{
	if(m_ptoEngaged)
	{
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
	}
	
	m_winchPawl->setDutyCyle(0.0f);
}

void Climber::disabledPeriodic()
{
	disengagePTO(true);
	
	m_componentData->climberData->setWinchPawlCurrent(0.0);
	
	winchPawlOff();
	
	m_componentData->climberData->setDesiredClimbingStep(INTENDED_IDLE );
	m_state = NOTHING;//Restart the routine, makes debugging easier.
	static int e = 0;
	e++;
//	if (e % 10 == 0)
//		AsyncPrinter::Printf("Climber Disabled\n");
	
	m_hooks->Set(false);
//	if (e % 50 == 0)
//	{
//		AsyncPrinter::Printf("Hook Change to %d\n", !m_pneumatics->GetHookState()	);
//		m_pneumatics->setHookPosition(!m_pneumatics->GetHookState());
//	}
	LCD::Instance()->Print(4, 0, true, "Climb: DISABLED");
}

void Climber::Configure()
{
	m_winch_current_threshold = m_config->Get<double> (m_configSection, "winchCurrentThreshold", 15.0);
	m_winch_engage_duty_cycle = m_config->Get<double> (m_configSection, "winchEngageDutyCycle", 0.05);
	m_timer_threshold = m_config->Get<int> (m_configSection, "timerThreshold", 25);
	m_drive_train_position_threshold = m_config->Get<int> (m_configSection, "driveTrainPositionThreshold", 42);
	m_servo_left_engaged_position = m_config->Get<int> (m_configSection, "leftServoEngaged", 1514);
	m_servo_right_engaged_position = m_config->Get<int> (m_configSection, "rightServoEngaged", 1514);
	m_servo_left_disengaged_position = m_config->Get<int> (m_configSection, "leftServoDisengaged", 905);
	m_servo_right_disengaged_position = m_config->Get<int> (m_configSection, "rightServoDisengaged", 905);
	m_pressure_scale = m_config->Get<double> (m_configSection, "pressure_scale", 1400);
	m_climbingLevelGearToothTicks[0] = m_config->Get<int>(m_configSection, "fromGroundLevelGearToothTicks", 40);
	m_climbingLevelGearToothTicks[1] = m_config->Get<int>(m_configSection, "fromTenPointLevelGearToothTicks", 130);
	m_climbingLevelGearToothTicks[2] = m_config->Get<int>(m_configSection, "fromTwentyPointLevelGearToothTicks", 130);
	
	m_armUpTicks = m_config->Get<int>(m_configSection, "armUpTicks", 155);
}

void Climber::Log()
{

}
