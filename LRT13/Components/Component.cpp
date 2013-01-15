#include "Component.h"

Component::Component(std::string name)
: m_actionData(data::ComponentData::GetInstance()),
  m_name(name)
{
	
}

std::string Component::GetName()
{
	return m_name;
}

list<Component::ComponentWithData> Component::CreateComponents()
{
	//TODO writeme!
	
}
