#include "Automation.h"

Automation::Automation()
{
	m_running = false;
}

Automation::~Automation()
{
	for (vector<Event*>::iterator it = start_events.begin(); it < start_events.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	start_events.clear();
	for (vector<Event*>::iterator it = abort_events.begin(); it < abort_events.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	abort_events.clear();
}

void Automation::Update()
{
	bool fired = true;
	for (vector<Event*>::iterator it = start_events.begin(); it < start_events.end(); it++)
	{
		if (!(*it)->Fired())
		{
			fired = false;
			break;
		}
	}
	if (fired && !m_running)
	{
		Start(*start_events.begin());
		m_running = true;
	}
	fired = true;
	for (vector<Event*>::iterator it = abort_events.begin(); it < abort_events.end(); it++)
	{
		if (!(*it)->Fired())
		{
			fired = false;
			break;
		}
	}
	if (fired && m_running)
	{
		Abort(*abort_events.begin());
		m_running = false;
	}
	if (m_running)
	{
		Run();
	}
}

void Automation::AddStartEvent(Event *trigger)
{
	start_events.push_back(trigger);
}

void Automation::AddAbortEvent(Event *trigger)
{
	abort_events.push_back(trigger);
}
