#include "Component.h"
#include "ComponentManager.h"

#warning needs implementation

Component::Component(std::string name, int dio, int requiresEnabledState)
: m_actionData(data::ComponentData::GetInstance()),
  m_name(name),
  m_manager(ComponentManager::Instance())
{
	m_manager->AddComponent(name, this, dio, requiresEnabledState);
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
