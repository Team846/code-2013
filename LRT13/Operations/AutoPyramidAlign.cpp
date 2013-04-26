#include "AutoPyramidAlign.h"

AutoPyramidAlign::AutoPyramidAlign()
	: AsyncProcess("")
{
	m_currentState = APA_State::DISABLED;
	m_startRobotDist = 0.0;
	m_startRobotAngle = 0.0;
	
	m_driveTrain = ComponentData::GetInstance()->drivetrainData;
	
	m_timer = 0;
}

AutoPyramidAlign::~AutoPyramidAlign()
{
	
}

void AutoPyramidAlign::Tick()
{
	switch(m_currentState)
	{
	case APA_State::DISABLED:
		break;
	case APA_State::GO_TO_CENTER:
		m_driveTrain->setControlMode(FORWARD, POSITION_CONTROL);
		m_driveTrain->setControlMode(TURN, VELOCITY_CONTROL);
		m_driveTrain->setRelativePositionSetpoint(FORWARD, m_driveToCenterDistance, m_driveSpeed);
		m_driveTrain->setVelocitySetpoint(TURN, 0.0);
		
		if(fabs(m_driveToCenterDistance - (DriveEncoders::GetInstance()->getRobotDist() - m_startRobotDist)) < 0.1)
		{
			m_timer = m_waitForSettleTicks;
			m_currentState = APA_State::WAIT_FOR_SETTLE;
		}
		break;
	case APA_State::WAIT_FOR_SETTLE:
		m_driveTrain->setControlMode(FORWARD, VELOCITY_CONTROL);
		m_driveTrain->setControlMode(TURN, VELOCITY_CONTROL);
		m_driveTrain->setVelocitySetpoint(FORWARD, 0.0);
		m_driveTrain->setVelocitySetpoint(TURN, 0.0);
		
		if(--m_timer <= 0)
		{
			m_currentState = APA_State::TURN;
			m_startRobotAngle = DriveEncoders::GetInstance()->getTurnAngle();
		}
		break;
	case APA_State::TURN:
		m_driveTrain->setControlMode(FORWARD, VELOCITY_CONTROL);
		m_driveTrain->setControlMode(TURN, VELOCITY_CONTROL);
		m_driveTrain->setVelocitySetpoint(FORWARD, 0.0);
		m_driveTrain->setRelativePositionSetpoint(TURN, m_turnAngle, m_turnSpeed);
		
		if(fabs((m_startRobotAngle + m_turnAngle) - DriveEncoders::GetInstance()->getTurnAngle()) <= 0.5)
		{
			m_driveTrain->setControlMode(FORWARD, VELOCITY_CONTROL);
			m_driveTrain->setControlMode(TURN, VELOCITY_CONTROL);
			m_driveTrain->setVelocitySetpoint(FORWARD, 0.0);
			m_driveTrain->setVelocitySetpoint(TURN, 0.0);
			
			m_currentState = APA_State::DISABLED;
		}
		break;
	}
}

void AutoPyramidAlign::Start()
{
	m_currentState = APA_State::GO_TO_CENTER;
	m_startRobotDist = DriveEncoders::GetInstance()->getRobotDist();
}

void AutoPyramidAlign::Reset()
{
	m_currentState = APA_State::DISABLED;
}

void AutoPyramidAlign::Configure()
{
	m_driveToCenterDistance = m_config->Get<double>("pyramid_align", "driveToCenterDistance", 90.0 / 2.0 - 33.0);
	m_turnAngle = m_config->Get<double>("pyramid_align", "turnAngle", -90.0);
	m_driveSpeed = m_config->Get<double>("pyramid_align", "driveSpeed", 0.3);
	m_turnSpeed = m_config->Get<double>("pyramid_align", "turnSpeed", 0.3);
	m_waitForSettleTicks = m_config->Get<int>("pyramid_align", "waitForSettleTicks", 10);
}
