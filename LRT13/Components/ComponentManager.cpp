#include "ComponentManager.h"

ComponentManager::ComponentManager()
{
	
}

ComponentManager::~ComponentManager()
{
	// for_each calls DeleteMapFntor<string, Component*>(pair<string, Component*>) for each element
	for_each(m_components.begin(),
			m_components.end(),
			Util::DeleteMapSecond<string, Component*>);
	
	m_components.clear();
}

void ComponentManager::Update()
{
	for(map<string, Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component* comp = it->second;
		
		if(comp == NULL || !comp->IsEnabled())
			continue;
		
		comp->Update();
	}
}

void ComponentManager::AddComponent(string id, Component* comp)
{
	m_components[id] = comp;
}

Component* ComponentManager::GetComponent(string id)
{
#warning should confirm if a component with id exists
	return m_components[id];
}

// the interesting part here is that the component decides if it's enabled or not.
// this is because sometimes a component will want to update when it's disabled.
// -tp
void ComponentManager::EnableComponent(string id)
{
	m_components[id]->onEnable();
	m_components[id]->Enable();
}

void ComponentManager::DisableComponent(string id)
{
	m_components[id]->onDisable();
	m_components[id]->Disable();
}

bool ComponentManager::IsComponentEnabled(string id)
{
	return m_components[id]->IsEnabled();
}
