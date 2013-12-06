#include "Automation.h"
#include "../Utils/AsyncPrinter.h"

map<ComponentData*, Automation*> Automation::allocated;

Automation::Automation()
{
	m_continueEvent = NULL;
}

Automation::~Automation()
{
}

Automation::Status Automation::Update()
{
	blocking.clear();
	AllocateResources();
	if (blocking.empty())
	{
		bool completed = Run();
		m_continueEvent = NULL;
		if(completed)
			return COMPLETED;
		else
			return IN_PROGRESS;
	}
	else
	{
		m_continueEvent = NULL;
		return RESOURCE_BUSY;
	}
}

void Automation::Continue(Event *trigger)
{
	m_continueEvent = trigger;
}

bool Automation::Continued()
{
	return m_continueEvent != NULL;
}

Event* Automation::GetContinueEvent()
{
	return m_continueEvent;
}

bool Automation::AllocateResource(ComponentData* resource)
{
	if (allocated.find(resource) == allocated.end())
	{
		allocated[resource] = this;
		return true;
	}
	blocking.insert(allocated[resource]);
	return false;
}

set<Automation*>& Automation::GetBlockingTasks()
{
	return blocking;
}

Automation* Automation::GetAllocation(ComponentData* resource)
{
	if (allocated.find(resource) != allocated.end())
		return allocated[resource];
	return NULL;
}

void Automation::DeallocateAll()
{
	allocated.clear();
}
