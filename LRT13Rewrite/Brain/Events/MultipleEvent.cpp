#include "MultipleEvent.h"

MultipleEvent::MultipleEvent()
{
}

MultipleEvent::MultipleEvent(vector<Event*> events)
{
	m_events = events;
}

MultipleEvent::~MultipleEvent()
{
}

bool MultipleEvent::CheckCondition()
{
	bool condition = true;
	for (vector<Event*>::iterator it = m_events.begin(); it < m_events.end(); it++)
	{
		if (!(*it)->CheckCondition())
			condition = false;
	}
	return condition;
}

void MultipleEvent::AddEvent(Event *event)
{
	m_events.push_back(event);
}
