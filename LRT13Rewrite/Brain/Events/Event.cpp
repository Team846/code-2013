#include "Event.h"

vector<Event*> Event::event_vector;

Event::Event()
{
	event_vector.push_back(this);
}

Event::~Event()
{
	for (vector<Automation*>::iterator it = start_listeners.begin(); it < start_listeners.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	for (vector<Automation*>::iterator it = abort_listeners.begin(); it < abort_listeners.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	for (vector<Automation*>::iterator it = continue_listeners.begin(); it < continue_listeners.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
}

void Event::AddStartListener(Automation* routine)
{
    start_listeners.push_back(routine);
}

void Event::AddAbortListener(Automation* routine)
{
    abort_listeners.push_back(routine);
}

void Event::AddContinueListener(Automation* routine)
{
    continue_listeners.push_back(routine);
}

vector<Automation*>& Event::GetStartListeners()
{
	return start_listeners;
}

vector<Automation*>& Event::GetAbortListeners()
{
	return abort_listeners;
}

vector<Automation*>& Event::GetContinueListeners()
{
	return continue_listeners;
}