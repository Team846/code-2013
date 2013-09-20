#include "AutoActions.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/RobotConfig.h"

AutoActions* AutoActions::m_instance = NULL;

AutoActions* AutoActions::Instance()
{
	if (m_instance == NULL)
		m_instance = new AutoActions();
	return m_instance;
}

void AutoActions::Finalize()
{
	DELETE(m_instance);
}

AutoActions::AutoActions()
{
	m_currentAction = NO_ADV_AUTOMATION;
	m_componentData = ComponentData::GetInstance();
}

AutoActions::~AutoActions()
{
	
}

void AutoActions::EnableAutoAimBackboard()
{
	m_currentAction = AUTO_AIM_BACKBOARD;
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
