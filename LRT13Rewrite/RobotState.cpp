#include "RobotState.h"

RobotState *RobotState::m_instance = NULL;

void RobotState::Initialize()
{
	if (m_instance == NULL)
		m_instance = new RobotState();
}

RobotState& RobotState::Instance()
{
	if (m_instance == NULL)
		m_instance = new RobotState();
	return *m_instance;
}

void RobotState::Finalize()
{
	delete m_instance;
}

RobotState::RobotState()
{
	m_totalTimer.Start();
	m_gameMode = DISABLED;
}

RobotState::~RobotState()
{
}

RobotState::Mode RobotState::GameMode()
{
	return m_gameMode;
}

RobotState::Mode RobotState::LastGameMode()
{
	return m_lastGameMode;
}

double RobotState::GameTime()
{
	return m_gameTimer.Get();
}

double RobotState::TotalTime()
{
	return m_totalTimer.Get();
}

void RobotState::Update()
{
	m_instance->m_lastGameMode = m_instance->m_gameMode;
	if (RobotBase::getInstance().IsDisabled())
	{
		m_instance->m_gameMode = DISABLED;
		m_instance->m_gameTimer.Stop();
		m_instance->m_gameTimer.Reset();
	}
	else if (RobotBase::getInstance().IsAutonomous())
	{
		m_instance->m_gameMode = AUTONOMOUS;
		m_instance->m_gameTimer.Start();
	}
	else if (RobotBase::getInstance().IsOperatorControl())
	{
		m_instance->m_gameMode = TELEOPERATED;
		m_instance->m_gameTimer.Start();
	}
}
