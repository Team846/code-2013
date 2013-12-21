#ifndef VALUE_CHANGE_EVENT_H_
#define VALUE_CHANGE_EVENT_H_

#include "Event.h"

/*!
 * @brief Event that fires when a variable changes to a value.
 */
template<typename T> class ValueChangeEvent : public Event
{
public:
	ValueChangeEvent(T* variable, T value);
	virtual ~ValueChangeEvent();
	
	virtual bool CheckCondition();
	
private:
	T* m_variable;
	T m_value;
};

#endif
