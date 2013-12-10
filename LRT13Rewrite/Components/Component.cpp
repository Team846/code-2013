#include "Component.h"
#include "Drivetrain.h"
#include "../RobotState.h"

vector<Component*> Component::component_vector;

Component::Component(const char *name, int di, bool requiresEnabledState) :
	m_name(name), m_digitalIn(di), m_requiresEnabled(requiresEnabledState)
{
	m_lastEnabled = false;
	
	printf("Created component: %s\n", name);
}

Component::~Component()
{
}

void Component::CreateComponents()
{
	component_vector.push_back(new Drivetrain());
}

void Component::DestroyComponents()
{
	for (vector<Component*>::iterator it = component_vector.begin(); it < component_vector.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	component_vector.clear();
}

void Component::UpdateAll()
{
	for (vector<Component*>::iterator it = component_vector.begin(); it < component_vector.end(); it++)
	{
		(*it)->Update();
	}
}

void Component::Update()
{
	if (RobotState::Instance().GameMode() != RobotState::DISABLED || !m_requiresEnabled)
	{
		if (m_digitalIn == -1 || DriverStation::GetInstance()->GetDigitalIn(m_digitalIn))
		{
			if (!m_lastEnabled)
				OnEnabled();
			UpdateEnabled();
			m_lastEnabled = true;
		}
		else
		{
			if (m_lastEnabled)
				OnDisabled();
			UpdateDisabled();
			m_lastEnabled = false;
		}
	}
	else
	{
		if (m_lastEnabled)
			OnDisabled();
		UpdateDisabled();
		m_lastEnabled = false;
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

