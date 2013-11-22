#ifndef JOYSTICK_MOVED_EVENT_H_
#define JOYSTICK_MOVED_EVENT_H_

#include "Event.h"
#include "../DebouncedJoystick.h"

class JoystickMovedEvent : public Event
{
public:
	JoystickMovedEvent(DebouncedJoystick *joystick, int axis = 0, float sensitivity = 0.02);
	virtual ~JoystickMovedEvent();
	
	virtual bool Fired();
	
	virtual void Update();
	
private:
	DebouncedJoystick *m_joystick;
	int m_axis;
	float m_sensitivity;
};

#endif
