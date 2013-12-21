#ifndef MULTIPLE_EVENT_H_
#define MULTIPLE_EVENT_H_

#include "Event.h"
#include <vector>

/*!
 * @brief Event that fires when the conditions of multiple events are met.
 */
class MultipleEvent : public Event
{
public:
	MultipleEvent();
	MultipleEvent(vector<Event*> events);
	virtual ~MultipleEvent();
	
	virtual bool CheckCondition();
	
	void AddEvent(Event *event);
	
private:
	vector<Event*> m_events;
};

#endif
