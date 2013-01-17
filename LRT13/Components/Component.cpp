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

list<Component::ComponentWithData> Component::CreateComponents()
{
	//TODO writeme!
	
}
