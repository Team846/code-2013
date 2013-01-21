#include "ComponentManager.h"
#include "ConfigLoader.h"
#include "Drivetrain.h"
#include "Collector.h"
#include "../ComponentData/RobotData.h"

ComponentManager::ComponentManager()
{
	
}

ComponentManager::~ComponentManager()
{
	// for_each calls DeleteMapFntor<string, FullComponent>(pair<string, Component*>) for each element
//	for_each(m_components.begin(),
//			m_components.end(),
//			Util::DeleteMapSecond<string, FullComponent>);
	
	m_components.clear();
}

ComponentManager* ComponentManager::Instance()
{
	if (m_instance == NULL)
		m_instance = new ComponentManager();
	return m_instance;
}

void ComponentManager::Update()
{
	for(map<string, FullComponent>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		FullComponent comp = it->second;
		
		if (RobotData::GetCurrentState() != RobotData::DISABLED || !comp.RequiresEnabledState)
		{
			if (m_ds->GetDigitalIn(comp.DIO) || comp.DIO == -1)
			{
				if(!comp.component->IsEnabled())
				{
					comp.component->onEnable();
					comp.component->Enable();
				}
			} else
			{
				if(comp.component->IsEnabled())
				{
					comp.component->onDisable();
					comp.component->Disable();
				}
			}
			comp.component->Update();
		} else
		{
			if(comp.component->IsEnabled())
			{
				comp.component->onDisable();
				comp.component->Disable();
			}
		}
	}
}

void ComponentManager::CreateComponents()
{
	ConfigLoader();
	Drivetrain();
	Collector();
}

void ComponentManager::AddComponent(string id, Component* comp, int dio, bool requiresEnabledState)
{
	FullComponent component;
	component.component = comp;
	component.DIO = dio;
	component.RequiresEnabledState = requiresEnabledState;
	m_components[id] = component;
}

Component* ComponentManager::GetComponent(string id)
{
#warning should confirm if a component with id exists
	return m_components[id].component;
}

void ComponentManager::EnableComponent(string id)
{
	m_components[id].component->onEnable();
	m_components[id].component->Enable();
}

void ComponentManager::DisableComponent(string id)
{
	m_components[id].component->onDisable();
	m_components[id].component->Disable();
}

bool ComponentManager::IsComponentEnabled(string id)
{
	return m_components[id].component->IsEnabled();
}
