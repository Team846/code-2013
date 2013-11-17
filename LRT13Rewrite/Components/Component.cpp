#include "Component.h"
#include "../RobotState.h"

vector<Component*> Component::component_vector;

Component::Component(const char *name, int di, bool requiresEnabledState) :
	m_name(name), m_digitalIn(di), m_requiresEnabled(requiresEnabledState)
{
	component_vector.push_back(this);
}

Component::~Component()
{
}

void Component::Update()
{
	if (RobotState::Instance().GameMode() != RobotState::DISABLED || !m_requiresEnabled)
	{
		if (m_digitalIn == -1 || DriverStation::GetInstance()->GetDigitalIn(m_digitalIn))
		{
			UpdateEnabled();
		}
		else
		{
			UpdateDisabled();
		}
	}
	else
	{
		UpdateDisabled();
	}
}

const char* Component::GetName()
{
	return m_name;
}

bool Component::EnableRequired()
{
	return m_requiresEnabled;
}

int Component::GetDigitalIn()
{
	return m_digitalIn;
}

