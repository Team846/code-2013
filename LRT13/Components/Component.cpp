#include "Component.h"

#warning needs implementation

Component::Component(std::string name)
: m_actionData(data::ComponentData::GetInstance()),
  m_name(name)
{
	
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
