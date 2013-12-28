#include "Event.h"

vector<Event*> Event::event_vector;

Event::Event()
{
	event_vector.push_back(this);
	lastFired = false;
}

Event::~Event()
{
}

bool Event::Fired()
{
	return CheckCondition() && !lastFired;
}

void Event::Update()
{
	lastFired = CheckCondition();
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
