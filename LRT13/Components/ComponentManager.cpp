#include "ComponentManager.h"
#include "ConfigLoader.h"
#include "Climber.h"
#include "Drivetrain.h"
#include "Collector.h"
#include "Shooter.h"
#include "WinchPawl.h"
#include "RoutineRecorder.h"
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
	AddComponent(new Climber()); //climber must be before drivetrain
	AddComponent(new Collector());//must be before the shooter and after climber 
	AddComponent(new WinchPawl());
	AddComponent(new Shooter()); //must be after climber
	AddComponent(new Drivetrain());
	AddComponent(new ConfigLoader());
#define MANOJ
#ifdef MANOJ
	AddComponent(new RoutineRecorder());
#endif
}

void ComponentManager::Update()
{
	for(map<string, Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component* comp = it->second;
		
		if(comp == NULL)
			continue;
		
		Profiler::BeginActivity(comp->GetName());
		
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
		
		Profiler::End(comp->GetName());
		
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
