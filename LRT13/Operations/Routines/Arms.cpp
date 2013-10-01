#include "Arms.h"
#include "../../ComponentData/ClimberData.h"

Arms::Arms(bool up)
{
	m_up = up;
}

void Arms::Run()
{
	if (m_up)
	{
		m_componentData->climberData->enableDebug();
		m_componentData->climberData->extendArm();
	}
	else
	{
		m_componentData->climberData->enableDebug();
		m_componentData->climberData->retractArm();
	}
}

bool Arms::Completed()
{
	return true;
}
