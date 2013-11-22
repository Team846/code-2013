#ifndef VALUE_CHANGE_EVENT_H_
#define VALUE_CHANGE_EVENT_H_

#include "Event.h"

template<typename T> class ValueChangeEvent : public Event
{
public:
	ValueChangeEvent(T* variable, T value);
	virtual ~ValueChangeEvent();
	
	virtual bool Fired();
	
	virtual void Update();
	
private:
	T* m_variable;
	T m_value;
	T m_lastValue;
};

#endif
