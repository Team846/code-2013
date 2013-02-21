#include "AutoActions.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/RobotConfig.h"

unsigned int AutoActions::m_counter = 0;
AutoActions::actions AutoActions::m_currentAction = NO_ADV_AUTOMATION;
	
AutoActions::AutoActions()
{
	m_componentData = ComponentData::GetInstance();
}

void AutoActions::AutoAimBackboard()
{
	m_currentAction = AUTO_AIM_BACKBOARD;
}

void AutoActions::AutoAimPyramid()
{
	m_currentAction = AUTO_AIM_PYRAMID;
}

void AutoActions::FeederStationTrack()
{
	m_currentAction = FEEDER_STATION_APPROACH;
}

void AutoActions::Reset()
{
	m_counter = 0;
	m_currentAction = IDLE;
}
