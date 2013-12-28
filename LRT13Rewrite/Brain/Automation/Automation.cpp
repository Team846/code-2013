#include "Automation.h"

map<ControlResource, int> Automation::allocated;
vector<Automation*> Automation::automation_vector;

Automation::Automation(const char *name, bool requiresAbortCycles, bool queueIfBlocked, bool restartable)
{
	m_startEvent = NULL;
	m_abortEvent = NULL;
	m_continueEvent = NULL;
	m_aborting = false;
	m_restartable = restartable;
	m_queueIfBlocked = queueIfBlocked;
	m_requiresAbortCycles = requiresAbortCycles;
	m_name = name;
	automation_vector.push_back(this);
}

Automation::~Automation()
{
}

bool Automation::Update()
{
	bool completed = Run();
	m_continueEvent = NULL;
	if (completed)
	{
		m_aborting = false;
		return true;
	}
	return false;
}

bool Automation::CheckResources()
{
	map<ControlResource, int> original = allocated;
	AllocateResources();
	bool success = true;
	for (map<ControlResource, int>::iterator it = allocated.begin(); it != allocated.end(); it++)
	{
		if (it->second > 1) // Resource allocated by this routine twice or resource already allocated by another routine
		{
			if (original.find(it->first) == original.end()) // Resource allocated twice
			{
				it->second = 1;
			}
			else // Resource already allocated by another routine
			{
				success = false;
			}
		}
	}
	if (!success)
	{
		allocated = original;
	}
	else
	{
		for (map<ControlResource, int>::iterator it = allocated.begin(); it != allocated.end(); it++)
		{
			if (original.find(it->first) == original.end())
				resources.push_back(it->first);
		}
	}
	return success;
}

bool Automation::StartAutomation(Event *trigger)
{
	m_startEvent = trigger;
	m_aborting = false;
	return Start();
}

bool Automation::AbortAutomation(Event *trigger)
{
	m_abortEvent = trigger;
	bool success = Abort();
	if (success)
	{
		if (RequiresAbortCycles())
		{
			m_aborting = true;
			return false;
		}
		m_startEvent = NULL;
		m_abortEvent = NULL;
		return true;
	}
	return false;
}

void Automation::ContinueAutomation(Event *trigger)
{
	m_continueEvent = trigger;
}

bool Automation::Continued()
{
	return m_continueEvent != NULL;
}

Event* Automation::GetStartEvent()
{
	return m_startEvent;
}

Event* Automation::GetAbortEvent()
{
	return m_abortEvent;
}

Event* Automation::GetContinueEvent()
{
	return m_continueEvent;
}

bool Automation::AllocateResource(ControlResource resource)
{
	if (allocated.find(resource) == allocated.end())
	{
		allocated[resource] = 1;
		return true;
	}
	allocated[resource]++;
	return false;
}

void Automation::DeallocateResources()
{
	for (vector<ControlResource>::iterator it = resources.begin(); it != resources.end(); it++)
	{
		if (allocated.find(*it) != allocated.end())
			allocated.erase(*it);
	}
	resources.clear();
}

bool Automation::GetAllocation(ControlResource resource)
{
	if (allocated.find(resource) != allocated.end())
		return allocated[resource] == 1;
	return false;
}

bool Automation::Aborting()
{
	return m_aborting;
}

bool Automation::IsRestartable()
{
	return m_restartable;
}

bool Automation::QueueIfBlocked()
{
	return m_queueIfBlocked;
}

bool Automation::RequiresAbortCycles()
{
	return m_requiresAbortCycles;
}

const char* Automation::GetName()
{
	return m_name;
}
