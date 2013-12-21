#include "Automation.h"

map<Automation::Resource, Automation*> Automation::allocated;

Automation::Automation(const char *name, bool restartable)
{
	m_continueEvent = NULL;
	m_restartable = restartable;
	m_name = name;
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

bool Automation::AllocateResource(Resource resource)
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

Automation* Automation::GetAllocation(Resource resource)
{
	if (allocated.find(resource) != allocated.end())
		return allocated[resource];
	return NULL;
}

void Automation::DeallocateAll()
{
	allocated.clear();
}

bool Automation::IsRestartable()
{
	return m_restartable;
}

const char* Automation::GetName()
{
	return m_name;
}
