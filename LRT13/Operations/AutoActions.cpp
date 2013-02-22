#include "AutoActions.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/RobotConfig.h"

AutoActions::AutoActions()
{
	m_componentData = ComponentData::GetInstance();
}

void AutoActions::EnableAutoAimBackboard()
{
	m_currentAction = AUTO_AIM_BACKBOARD;
}

void AutoActions::EnableAutoAimPyramid()
{
	m_currentAction = AUTO_AIM_PYRAMID;
}

void AutoActions::EnableFeederStationTrack()
{
	m_currentAction = FEEDER_STATION_APPROACH;
}

void AutoActions::Reset()
{
	m_counter = 0;
	m_currentAction = NO_ADV_AUTOMATION;
}

AutoActions::actions AutoActions::GetCurrentAutonomousAction()
{
	return m_currentAction;
}

void AutoActions::SetRoutine(routines routine)
{
	m_currentAutoRoutine = routine;
}

AutoActions::routines AutoActions::GetRoutine()
{
	return m_currentAutoRoutine;
}

void AutoActions::setNumFrisbeesToPickUp(int numFrisbeesToPickUp)
{
	m_numFrisbeesToPickUp = numFrisbeesToPickUp;
}

int AutoActions::getNumFrisbeesToPickUp()
{
	return m_numFrisbeesToPickUp;
}
