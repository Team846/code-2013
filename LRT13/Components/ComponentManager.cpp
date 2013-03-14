#include "ComponentManager.h"
#include "ConfigLoader.h"
#include "Drivetrain.h"
#include "Collector.h"
#include "Shooter.h"
#include "../ComponentData/RobotData.h"
#include "../Utils/AsyncPrinter.h"
#include "../Pneumatics/Pneumatics.h"

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

void ComponentManager::CreateComponents()
{
	AddComponent(new Drivetrain());
	AddComponent(new Shooter());
	AddComponent(new Collector());
	AddComponent(new ConfigLoader());
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
			if (comp->GetDIO() == -1 || DriverStation::GetInstance()->GetDigitalIn(comp->GetDIO()))
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
				comp->enabledPeriodic();
			}
			else
			{
				comp->disabledPeriodic();
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
			comp->enabledPeriodic();
		}
		else
		{
			comp->disabledPeriodic();
		}
	}
}

void ComponentManager::AddComponent(Component* comp)
{
	AsyncPrinter::Printf("Adding component: %s\n", comp->GetName().c_str());
	AddComponent(comp->GetName(), comp);
}

void ComponentManager::AddComponent(string id, Component* comp)
{
	m_components[id] = comp;
}

Component* ComponentManager::GetComponent(string id)
{
	if(HasComponent(id))
	{
		return m_components[id];
	}
	else
	{
		AsyncPrinter::Printf("ComponentManager::GetComponent(): Invalid Reference to Component: %s\n", id.c_str());
		return NULL;
	}
}

void ComponentManager::EnableComponent(string id)
{
	if(HasComponent(id))
	{
		m_components[id]->Enable();
	}
	else
	{
		AsyncPrinter::Printf("ComponentManager::EnableComponent(): Invalid Reference to Component: %s\n", id.c_str());
	}
}

void ComponentManager::DisableComponent(string id)
{
	if(HasComponent(id))
	{
		m_components[id]->Disable();
	}
	else
	{
		AsyncPrinter::Printf("ComponentManager::DisableComponent(): Invalid Reference to Component: %s\n", id.c_str());
	}
}

bool ComponentManager::IsComponentEnabled(string id)
{
	if(HasComponent(id))
	{
		return m_components[id]->IsEnabled();
	}
	else
	{
		AsyncPrinter::Printf("ComponentManager::IsComponentEnabled(): Invalid Reference to Component: %s\n", id.c_str());
	}
	return false;
}

bool ComponentManager::HasComponent(string id)
{
	map<string, Component*>::iterator it = m_components.find(id);
	if(it == m_components.end())
	{
		return false;
	}
	return true;
}
