#include "Event.h"

vector<Event*> Event::event_vector;

Event::Event()
{
	event_vector.push_back(this);
}

Event::~Event()
{
	
}

void Event::UpdateEvents()
{
	for (vector<Event*>::iterator it = event_vector.begin(); it < event_vector.end(); it++)
	{
		(*it)->Update();
	}
}
