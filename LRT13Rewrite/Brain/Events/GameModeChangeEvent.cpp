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
	if (Event::Fired())
	{
		if (m_from && RobotState::Instance().LastGameMode() != m_fromMode)
		{
			return false;
		}
		return true;
	}
	return false;
}

bool GameModeChangeEvent::CheckCondition()
{
	return RobotState::Instance().GameMode() == m_toMode;
}

