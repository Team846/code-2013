#include "ComponentManager.h"
#include "ConfigLoader.h"
#include "Drivetrain.h"
#include "Collector.h"
#include "../ComponentData/RobotData.h"
#include "../Utils/AsyncPrinter.h"

ComponentManager::ComponentManager()
{
	
}

ComponentManager::~ComponentManager()
{
	// for_each calls DeleteMapFntor<string, Component>(pair<string, Component*>) for each element
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
		
		if(comp == NULL)
			continue;
		
		if (RobotData::GetCurrentState() != RobotData::DISABLED || !comp->EnableRequired())
		{
			if (DriverStation::GetInstance()->GetDigitalIn(comp->GetDIO()) || comp->GetDIO() == -1)
			{
				if(!comp->IsEnabled())
				{
					comp->Enable();
				}
			}
			else
			{
				if(comp->IsEnabled())
				{
					comp->Disable();
				}
			}
			
			if(comp->IsEnabled())
			{
				comp->whenEnabled();
			}
			else
			{
				comp->whenDisabled();
			}
		} 
		else
		{
			if(comp->IsEnabled())
			{
				comp->Disable();
			}
		}
		
		if(comp->IsEnabled())
		{
			comp->whenEnabled();
		}
		else
		{
			comp->whenDisabled();
		}
	}
}

void ComponentManager::AddComponent(Component* comp)
{
	AsyncPrinter::DbgPrint("Adding component: %s\n", comp->GetName().c_str());
	AddComponent(comp->GetName(), comp);
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

void ComponentManager::EnableComponent(string id)
{
	m_components[id]->Enable();
}

void ComponentManager::DisableComponent(string id)
{
	m_components[id]->Disable();
}

bool ComponentManager::IsComponentEnabled(string id)
{
	return m_components[id]->IsEnabled();
}
