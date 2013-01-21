#include "Component.h"
#include "ComponentManager.h"

#warning needs implementation

Component::Component(std::string name, int dio, bool requiresEnabledState)
: m_actionData(data::ComponentData::GetInstance()),
  m_name(name)
{
	m_DIO = dio;
	m_requiresEnabled = requiresEnabledState;
}

Component::~Component()
{
}

std::string Component::GetName()
{
	return m_name;
}

void Component::Enable()
{
	m_enabled = true;
}

void Component::Disable()
{
	m_enabled = false;
}

bool Component::IsEnabled()
{
	return m_enabled;
}

bool Component::EnableRequired()
{
	return m_requiresEnabled;
}

int Component::GetDIO()
{
	return m_DIO;
}
