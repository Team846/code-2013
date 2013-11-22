#include "GameModeChangeEvent.h"

GameModeChangeEvent::GameModeChangeEvent(RobotState::Mode toMode)
{
	m_toMode = toMode;
	m_from = false;
}

GameModeChangeEvent::GameModeChangeEvent(RobotState::Mode fromMode, RobotState::Mode toMode)
{
	m_toMode = toMode;
	m_fromMode = fromMode;
	m_from = true;
}
	
GameModeChangeEvent::~GameModeChangeEvent()
{
}

bool GameModeChangeEvent::Fired()
{
	if (RobotState::Instance().GameMode() == m_toMode)
	{
		if (m_from && RobotState::Instance().LastGameMode() == m_fromMode)
		{
			return true;
		}
		else if (!m_from && RobotState::Instance().LastGameMode() != m_toMode)
		{
			return true;
		}
	}
	return false;
}

void GameModeChangeEvent::Update()
{
}

