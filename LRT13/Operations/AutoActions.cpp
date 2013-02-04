#include "AutoActions.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/RobotConfig.h"

unsigned int AutoActions::m_counter = 0;
AutoActions::actions AutoActions::m_currentAction = AUTO_AIM;
	
AutoActions::AutoActions()
{
	m_componentData = ComponentData::GetInstance();
}

bool AutoActions::AutoAim()
{
	if (m_currentAction != AUTO_AIM)
	{
		m_counter = 0;
		m_currentAction = AUTO_AIM;
	}
	m_counter++;
	if (m_counter >= RobotConfig::AutoActions::AUTO_AIM_TIMEOUT)
	{
		return true;
	}
	return true;
}

bool AutoActions::AutoClimb()
{
	if (m_currentAction != AUTO_CLIMB)
	{
		m_counter = 0;
		m_currentAction = AUTO_CLIMB;
	}
	m_counter++;
	return true;
}

bool AutoActions::DiscTrack()
{
	if (m_currentAction != DISC_TRACK)
	{
		m_counter = 0;
		m_currentAction = DISC_TRACK;
	}
	m_counter++;
	return true;
}

void AutoActions::Reset()
{
	m_counter = 0;
	m_currentAction = AUTO_AIM;
}
