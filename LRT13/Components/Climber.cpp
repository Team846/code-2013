#include "Climber.h"
#include "../ComponentData/LEDIndicatorData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

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
	m_climbing_level = 0;
	m_pneumatics = Pneumatics::Instance();
	m_state = NOTHING;
	m_driving_encoders = DriveEncoders::GetInstance();
	m_climbing_level = 0;
	m_paused = false;
	m_previous_state = NOTHING;
	m_winch_gear_tooth.Start();
	
	m_winchPawl = m_componentData->winchPawlData;
	m_climberData = m_componentData->climberData;
	m_shooterData = m_componentData->shooterData;
	
	m_driveSpeed = 0.0;
	
	m_ptoEngaged = false;
	
	m_waitGoToState = INACTIVE;
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
	m_logFile << "Time: " << Timer::GetFPGATimestamp() << endl;
	
	const double curr = m_winchPawl->getMotorCurrent();
	
	m_componentData->climberData->setWinchPawlCurrent(curr);
	
	m_paused = false; //we pause

//	if(m_state != INACTIVE && m_winchPawl->isWinchPawlTimedOut())
//	{
//		m_climberData->setShouldPotentiallyAbort(true);
//	}
	
	if (m_state != INACTIVE && m_componentData->climberData->shouldPotentiallyAbort())
	{
		m_state = INACTIVE;
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		
		disengagePTO();
		
		m_winchPawl->setDutyCyle(0.0f);
		
		m_paused = false;
	}
	
	static bool wasDebugging = false;
	if (m_componentData->climberData->shouldDebug())
	{
		m_climberData->setCurrentState(DEBUG_MODE);
		AsyncPrinter::Printf("Debug\n");
		if (m_componentData->climberData->shouldChangeAngleState())
		{
			if (m_componentData->shooterData->ShouldLauncherBeHigh())
				m_componentData->shooterData->SetLauncherAngleLow();
			else
				m_componentData->shooterData->SetLauncherAngleHigh();
		}
		
		if (m_componentData->climberData->shouldWinchPawlGoUp())
		{
			AsyncPrinter::Printf("winchpawl up %d\n", m_winch_gear_tooth.Get());
			m_winchPawl->setDutyCyle(1.0f * m_winchPawlUpDirection);
		}
		else if (m_componentData->climberData->shouldWinchPawlGoDown())
		{
			AsyncPrinter::Printf("winch pawl down %d\n", m_winch_gear_tooth.Get());
			m_winchPawl->setDutyCyle(1.0f * m_winchPawlDownDirection);
		}
		else
		{
//			AsyncPrinter::Printf("You failed a tlife!\n");
			m_winchPawl->setDutyCyle(0.0);
		}
		
		if (m_componentData->climberData->shouldPTOChangeDisengage())
		{
			disengagePTO();
		}
		else if (m_componentData->climberData->shouldPTOChangeEngage())
		{
			engagePTO();
		}
		
		if(m_climberData->shouldChangeHooks())
		{
			m_pneumatics->setHookPosition(!m_pneumatics->GetHookState());
		}
		
		if(m_climberData->shouldChangeArm())
		{
			m_pneumatics->setClimberArm(!m_pneumatics->GetClimberState(), true);
		}
//		m_pneumatics->setHookPosition(m_componentData->climberData->shouldExtendHooks());
//		m_pneumatics->setClimberArm(m_componentData->climberData->shouldExtendArm());
//		AsyncPrinter::Printf("Still alive %d\n", GetFPGATime());
		return;
	}

	state desiredStep = m_climberData->getDesiredState();
	
	if(desiredStep > NOTHING)
	{
		m_state = WAIT;
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
//	if (e % 10 == 0)
//	{
//		string str = "no_state";
//	switch(m_state)
//	{
//	case IDLE:
//		AsyncPrinter::Printf("Idle\n");
//		str = "idle";
//		break;
//	case ARM_UP_INITIAL:
//		AsyncPrinter::Printf("Arm up\n");
//		str = "arm up";
//		break;
//	case WAIT:
//		AsyncPrinter::Printf("Wait\n");
//		str = "wait";
//		break;
//	case ARM_DOWN:
//		AsyncPrinter::Printf("Arm Down\n");
//		str = "arm down";
//		break;
//	case DUMB_ENGAGE_PTO:
//		AsyncPrinter::Printf("Dumb Engage PTO\n");
//		str = "dumb engage pto";
//		break;
//	case ENGAGE_PTO:
//		AsyncPrinter::Printf("Engage PTO\n");
//		AsyncPrinter::Printf("Left %d, rigt %d\n", m_digital_input_left.Get(), m_digital_input_right.Get());
//		str = "engage pto";
//		break;
//	case WINCH_UP:
//		AsyncPrinter::Printf("WINCH_UP\n");
//		str = "winch up";
//		break;
//	case ENGAGE_HOOKS:
//		AsyncPrinter::Printf("ENGAGE HOOKS\n");
//		str = "engage hooks";
//		break;
//	case DISENGAGE_PTO:
//		m_state = ARM_UP_FINAL;
//		str = "disengage pto";
//		break;
//	case ARM_UP_FINAL:
//		AsyncPrinter::Printf("ARM_UP_FINAl\n");
//		m_state = WAIT;
//		str = "arm up final";
//		break;
//	}
//	
//	NetBuffer buff;
//	
//	buff.Write((char)MessageType::ROBOT_TELEMETRY);
//	buff.Write(str);
//	
//	SmarterDashboard::Instance()->EnqueueMessage(&buff, NetChannel::NET_RELIABLE, 2);
//	}
	
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
	
	if(m_climbing_level >= 3)
		return; // don't try it, sonny
	
	string m_stateString = "???";
	
	switch(m_state)
	{
	case NOTHING:
		if(m_climberData->shouldContinueClimbing())
			m_state = INACTIVE;
	case INACTIVE:
		m_stateString = "INACTIVE";
		
		disengagePTO();
		
		if(m_climbing_level == GROUND)
		{
			m_pneumatics->setHookPosition(RETRACTED, true);
			m_pneumatics->setClimberArm(RETRACTED, true);
		}
		
		m_winchPawl->setDutyCyle(0.0F);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = BEGIN;
		
		m_driveSpeed = 0.0;
		
		break;
	case BEGIN:
		m_stateString = "BEGIN";
		
		m_pneumatics->setClimberArm(EXTENDED, true);
		
		m_shooterData->SetLauncherAngleLow();
		
		if(m_climberData->shouldContinueClimbing())
			m_state = m_climbing_level > GROUND ? ARM_UP : COLLECTOR_DOWN;
		break;
	case ARM_UP:
		m_stateString = "ARM_UP";
		
		// let the operator / autoclimb do its job!
		
		if(m_climberData->shouldContinueClimbing())
			m_state = COLLECTOR_DOWN;
		
		break;
	case COLLECTOR_DOWN:
		m_stateString = "COLLECTOR_DOWN";
		
		m_shooterData->SetLauncherAngleLow();
		m_pneumatics->setCollector(EXTENDED, true);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = LINE_UP;
	case LINE_UP:
		m_stateString = "LINE_UP";

		if(m_climbing_level > GROUND)
			m_shooterData->SetLauncherAngleHigh();
		else
		{
			m_shooterData->SetLauncherAngleLow();
		}
		
		// let the operator / autoclimb do its job!
		
		if(m_climberData->shouldContinueClimbing())
			m_state = ARM_DOWN_PREPARE;
		break;
	case ARM_DOWN_PREPARE:
		m_stateString = "ARM_DOWN_PREPARE";
		
		m_timer = 10;
		m_state = ARM_DOWN;
		break;
	case ARM_DOWN:
		m_stateString = "ARM_DOWN";
		
		// m_timer is set to 10 in ARM_DOWN_PREPARE
		
		m_winchPawl->setDutyCyle(1.0f * m_winchPawlDownDirection);
		
		if(curr > m_winch_current_threshold)
			--m_timer;
		
		if(m_timer <= 0)
		{
//			if (m_climbing_level == GROUND)
//				m_winchPawl->setDutyCyle(m_winch_engage_duty_cycle * m_winchPawlDownDirection);
//			else
				m_winchPawl->setDutyCyle(0.0);
			
			if(m_climberData->shouldContinueClimbing())
				m_state = CLIMB_PREPARE;
		}
		break;
	case CLIMB_PREPARE:
		m_stateString = "CLIMB_PREPARE";
		
		m_pneumatics->setHookPosition(RETRACTED, true);
		m_shooterData->SetLauncherAngleLow();
		if (m_climbing_level > GROUND)
			m_pneumatics->setCollector(RETRACTED, true);
		
		engagePTO();
		
		m_drive_train_position = m_winch_gear_tooth.Get();
		
		m_logFile << "m_drive_train_position: " << m_drive_train_position << endl;
		
		m_driveSpeed = 0.0;

		//m_state = CLIMB;
		
		if(m_climberData->shouldContinueClimbing())
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
		
		if (m_winch_gear_tooth.Get() - m_drive_train_position > m_climbingLevelGearToothTicks[m_climbing_level])//Should be about 42
		{
			m_logFile << "stopping" << endl;
			
			m_winchPawl->setDutyCyle(0.0); // all right, paul, you can take a breather now
			
			// stop the drivetrain now
			m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
			m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
			m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
			
			if(m_climberData->shouldContinueClimbing())
				m_state = TURN_WINCH_PAWL_OFF;
		}
		break;
	case TURN_WINCH_PAWL_OFF:
		m_stateString = "TURN_WINCH_PAWL_OFF";
		
		m_winchPawl->setDutyCyle(0.0F);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = EXTEND_HOOKS;
		
		break;
	case EXTEND_HOOKS:
		m_stateString = "EXTEND_HOOKS";
		
		m_pneumatics->setHookPosition(EXTENDED, true);
		
		if(m_climberData->shouldContinueClimbing())
		{
			m_state = CLIMBED;
		}
		break;
	case CLIMBED:
		m_stateString = "CLIMBED";
		
		m_climbing_level++;
		
		//if(m_climberData->shouldContinueClimbing())
			m_state = INACTIVE;
		break;
	case WAIT: // this should never be set by the routine -- only the human operator should set it
		m_stateString = "WAIT";
		
		AsyncPrinter::Printf("Waiting for next state: %d\n", m_waitGoToState);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = m_waitGoToState;
		break;
	case RESET_FOR_INACTIVE:
		m_stateString = "RESET_INACTIVE";

		m_winchPawl->setDutyCyle(0.0F);
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = INACTIVE;
		break;
	case RESET_FOR_BEGIN:
		m_stateString = "RESET_BEGIN";

		disengagePTO();
				
		if(m_climbing_level == GROUND)
		{
			m_pneumatics->setHookPosition(RETRACTED, true);
			m_pneumatics->setClimberArm(RETRACTED, true);
		}
		
		m_winchPawl->setDutyCyle(0.0F);
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = BEGIN;
		break;
	case RESET_FOR_LINE_UP:
		m_stateString = "RESET_LINE_UP";

		disengagePTO();
		
		if(m_climbing_level == GROUND)
		{
			m_pneumatics->setHookPosition(RETRACTED, true);
		}
		m_pneumatics->setClimberArm(EXTENDED, true);

		if(m_climbing_level > GROUND)
			m_shooterData->SetLauncherAngleHigh();
		else
			m_shooterData->SetLauncherAngleLow();
				
		m_winchPawl->setDutyCyle(0.0F);
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = ARM_UP;
		break;
	case RESET_FOR_ARM_DOWN_PREPARE:
		m_stateString = "RESET_ARM_DOWN_PREPARE";
		
		disengagePTO();
				
		if(m_climbing_level == GROUND)
		{
			m_pneumatics->setHookPosition(RETRACTED, true);
		}
		m_pneumatics->setClimberArm(EXTENDED, true);

		if(m_climbing_level > GROUND)
			m_shooterData->SetLauncherAngleHigh();
		else
			m_shooterData->SetLauncherAngleLow();
				
		m_winchPawl->setDutyCyle(0.0F);
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = ARM_DOWN_PREPARE;
		break;
	case RESET_FOR_ARM_DOWN:
		m_stateString = "RESET_ARM_DOWN";
		
		disengagePTO();
						
		if(m_climbing_level == GROUND)
		{
			m_pneumatics->setHookPosition(RETRACTED, true);
		}
		m_pneumatics->setClimberArm(EXTENDED, true);

		if(m_climbing_level > GROUND)
			m_shooterData->SetLauncherAngleHigh();
		else
			m_shooterData->SetLauncherAngleLow();
				
		m_winchPawl->setDutyCyle(0.0F);
		m_timer = 10;
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = ARM_DOWN;
		break;
	case RESET_FOR_CLIMB_PREPARE:
		m_stateString = "RESET_CLIMB_PREPARE";
		
		disengagePTO();
								
		if(m_climbing_level == GROUND)
		{
			m_pneumatics->setHookPosition(RETRACTED, true);
		}
		m_pneumatics->setClimberArm(EXTENDED, true);

		if(m_climbing_level > GROUND)
			m_shooterData->SetLauncherAngleHigh();
		else
			m_shooterData->SetLauncherAngleLow();
				
		if (m_climbing_level == GROUND)
			m_winchPawl->setDutyCyle(m_winch_engage_duty_cycle * m_winchPawlDownDirection);
		else
			m_winchPawl->setDutyCyle(0.0);
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = CLIMB_PREPARE;
		break;
	case RESET_FOR_CLIMB:
		m_stateString = "RESET_CLIMB";
		
		m_pneumatics->setClimberArm(EXTENDED);
		m_pneumatics->setHookPosition(RETRACTED);
		m_shooterData->SetLauncherAngleLow();
		
		engagePTO();
		
		m_drive_train_position = m_winch_gear_tooth.Get();
		
		m_driveSpeed = 0.0;
		
		if (m_climbing_level == GROUND)
			m_winchPawl->setDutyCyle(m_winch_engage_duty_cycle * m_winchPawlDownDirection);
		else
			m_winchPawl->setDutyCyle(0.0);
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
				
		if(m_climberData->shouldContinueClimbing())
			m_state = CLIMB;
		break;
	case RESET_FOR_TURN_WINCH_PAWL_OFF:
		m_stateString = "RESET_TURN_WINCH_PAWL_OFF";
		
		m_pneumatics->setClimberArm(EXTENDED);
		m_pneumatics->setHookPosition(RETRACTED);
		m_shooterData->SetLauncherAngleLow();
		
		engagePTO();
		
		m_winchPawl->setDutyCyle(0.0);
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = TURN_WINCH_PAWL_OFF;
		break;
	case RESET_FOR_EXTEND_HOOKS:
		m_stateString = "RESET_EXTEND_HOOKS";
		
		m_pneumatics->setClimberArm(EXTENDED);
		m_pneumatics->setHookPosition(RETRACTED);
		m_shooterData->SetLauncherAngleLow();
		
		engagePTO();
		
		m_winchPawl->setDutyCyle(0.0);
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);

		if(m_climberData->shouldContinueClimbing())
			m_state = EXTEND_HOOKS;
		break;
	case RESET_FOR_CLIMBED:
		m_stateString = "RESET_CLIMBED";

		m_pneumatics->setClimberArm(EXTENDED);
		m_pneumatics->setHookPosition(EXTENDED);
		m_shooterData->SetLauncherAngleLow();
		
		engagePTO();
		
		m_winchPawl->setDutyCyle(0.0);
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		if(m_climberData->shouldContinueClimbing())
			m_state = CLIMBED;
		break;
	}
	
	static int printCounter = 0;
	
	if(printCounter++ % 20 == 0 || m_state != m_previous_state)
		AsyncPrinter::Printf("Climb state: %s\n", m_stateString.c_str());
	
	SmarterDashboard::Instance()->SetTelemetryData<string>(TelemetryType::CLIMBER_STATE, m_stateString);
	
	m_logFile << "State: " << m_stateString << endl;
	m_logFile << "Winch Pawl Current: " << m_winchPawl->getMotorCurrent() << " amps" << endl;
	m_logFile << "Winch Pawl Requested Out: " << m_winchPawl->getDutyCycle() << endl;
	m_logFile << "Drivetrain Forward Open Loop Out: " << m_componentData->drivetrainData->getOpenLoopOutput(FORWARD) << endl;
	m_logFile << endl;
	
	m_logFile.flush();
	
	/*
	switch (m_state)
	{
	case IDLE:
		m_pneumatics->setHookPosition(false);
		m_climbing_level  = 0;                       
		m_pneumatics->setClimberArm(RETRACTED, true);
		m_winchPawl->setDutyCyle(0.0);
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
			
		}
		break;
	case ARM_DOWN:
#define DIRECTION 1
		
		m_pneumatics->setClimberArm(EXTENDED);
		m_componentData->shooterData->SetLauncherAngleLow();
		m_winchPawl->setDutyCyle(-1.0 * DIRECTION);
		curr = m_winchPawl->getMotorCurrent();
		if (curr > m_winch_current_threshold)
		{
			m_timer--;
			if (m_timer < 0)
			{
				if (m_climbing_level < 1)
					m_winchPawl->setDutyCyle(-m_winch_engage_duty_cycle * DIRECTION);
				else
					m_winchPawl->setDutyCyle(0.0);
				m_state = ENGAGE_PTO;
				m_hasStartedEngaging = false;
				m_timer = 25;
				
//				m_timer = 5;//for unlocking the pawl
//				m_state = WINCH_UP;
//				driveSpeed = 0.0;
//				m_drive_train_position = m_driving_encoders->getRobotDist(); //awk 
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
		AsyncPrinter::Printf("Climb\n");
		m_servo_left.SetEnabled(true);
		m_servo_right.SetEnabled(true);
		m_servo_left.SetMicroseconds(m_servo_left_engaged_position);
		m_servo_right.SetMicroseconds(m_servo_right_engaged_position);
		engaged = false;
		m_state = WINCH_UP;
		m_drive_train_position = m_winch_gear_tooth.Get();
		m_timer = 25;
		driveSpeed = 0;
//		m_pneumatics->setClimberArm(false);
//		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
//		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
//		if (m_climbing_level < 1)
//			m_winch_worm.SetDutyCycle(-m_winch_engage_duty_cycle);
//		else
//			m_winch_worm.SetDutyCycle(0.0);
//
//		if (m_digital_input_left.Get() ^ m_digital_input_right.Get()) // only one side engaged
//		{
//			if (m_digital_input_left.Get()) // left side engaged
//				m_side_engaged = 1;
//			else
//				// right side engaged
//				m_side_engaged = -1;
//
//			if (m_winch_worm.GetOutputCurrent() > m_winch_current_threshold)
//			{
//				m_hasStartedEngaging = true;
//			}
//			else if (m_hasStartedEngaging)
//			{
//				m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
//				m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.09 * m_side_engaged);
//			}
//		}
//		else if (!m_digital_input_left.Get() && !m_digital_input_left.Get()) // both engaged
//		{
//			m_timer = 5;//for unlocking the pawl
//			m_state = WINCH_UP;
//			driveSpeed = 0.0;
//			m_drive_train_position = m_driving_encoders->getRobotDist(); //awk units (2/3 in)
//		}
//		else
//		{
//			m_hasStartedEngaging = false;
//			m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
//			m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
//		}
//		// TODO: disable servo afterwards
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
			m_winchPawl->setDutyCyle(1.0 * DIRECTION);
		}
		break;
	case WINCH_UP:
//		m_pneumatics->setClimberArm(RETRACTED);
//		m_componentData->shooterData->SetLauncherAngleLow();
		m_winchPawl->setDutyCyle(-1.0 * DIRECTION);
		//30:1 on worm, 4:1, 19k rpm, 158.33333333333333333333333333333 max
		//11:1, 14-54, 5400/ 42.428571428571428571428571428571  = 130
		if (m_climbing_level < 1 && false)
		{
			driveSpeed = DIRECTION;
		}
		else if (fabs(driveSpeed) < 0.99)
		{
			driveSpeed += 0.05 * DIRECTION;
		}
		
//		m_timer--;
//		if (m_timer < 0)
		{
			m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
			m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, driveSpeed);
			m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		}
		
		if (false)//!engaged && m_digital_input_left.Get() && m_digital_input_left.Get())
		
		{
			m_drive_train_position = m_driving_encoders->getRobotDist(); //awk units (2/3 in)
			engaged = true;
		}
		else if (m_winch_gear_tooth.Get() - m_drive_train_position > 39)//SHould be about 42
		{
			m_winchPawl->setDutyCyle(0.0);
			m_state = IDLE;
			m_timer = 0;
//			m_state = ENGAGE_HOOKS;
			m_climbing_level++;
			m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
			m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
			m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		}
		else
		{
			AsyncPrinter::Printf("encoder dist: %.2f speed %.2f\n", m_winch_gear_tooth.Get() - m_drive_train_position, driveSpeed);
//			AsyncPrinter::Printf("dist: %.2f, l: %d, r: %d\n", fabs(m_driving_encoders->getRobotDist() - m_drive_train_position) , m_digital_input_left.Get(), m_digital_input_right.Get());
		}
		break;
	case ENGAGE_HOOKS:
		m_pneumatics->setHookPosition(true);

		driveSpeed = 0;
		if (++m_timer > 25)
		{
			if (m_climbing_level < 3)
			{
//				m_state = DISENGAGE_PTO;
				m_state = ARM_UP_FINAL;
//				m_climbing_level++;
				m_gear_tooth_ticks_position = m_winch_gear_tooth.Get();
//				m_	timer = 0;
			}
			else
			{
//				m_state = IDLE;
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
		m_winchPawl->setDutyCyle(-1.0 * DIRECTION);
		
		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
		m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, DIRECTION);
		m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
		
		
		if (m_winch_gear_tooth.Get() - m_gear_tooth_ticks_position > 80)
		{
			m_winchPawl->setDutyCyle(0.0);
			m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
			m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
			m_componentData->drivetrainData->setOpenLoopOutput(TURN, 0.0);
//			m_state = WINCH_UP;
		}
		break;
	}
	m_paused = false; //we pause
	if (m_previous_state != m_state)
	{
		//don't pause for now
//		m_paused = true; //we pause
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
	*/

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

void Climber::disabledPeriodic()
{
	m_componentData->climberData->setWinchPawlCurrent(0.0);
	
	m_winchPawl->setDutyCyle(0.0f);
	
	m_componentData->climberData->setDesiredClimbingStep(INTENDED_IDLE );
	m_state = INACTIVE;//Restart the routine, makes debugging easier.
	m_climbing_level = GROUND;
	static int e = 0;
	e++;
//	if (e % 10 == 0)
//		AsyncPrinter::Printf("Climber Disabled\n");
	
	disengagePTO(true);
	
	m_pneumatics->setHookPosition(false);
//	if (e % 50 == 0)
//	{
//		AsyncPrinter::Printf("Hook Change to %d\n", !m_pneumatics->GetHookState()	);
//		m_pneumatics->setHookPosition(!m_pneumatics->GetHookState());
//	}
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
	
	m_climbingLevelGearToothTicks[0] = m_config->Get(m_configSection, "fromGroundLevelGearToothTicks", 40);
	m_climbingLevelGearToothTicks[1] = m_config->Get(m_configSection, "fromTenPointLevelGearToothTicks", 130);
	m_climbingLevelGearToothTicks[2] = m_config->Get(m_configSection, "fromTwentyPointLevelGearToothTicks", 130);
}

void Climber::Log()
{

}
