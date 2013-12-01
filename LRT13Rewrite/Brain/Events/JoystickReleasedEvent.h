#ifndef JOYSTICK_RELEASED_EVENT_H_
#define JOYSTICK_RELEASED_EVENT_H_

#include "Event.h"
#include "../../DriverStation/DebouncedJoystick.h"

class JoystickReleasedEvent : public Event
{
public:
	JoystickReleasedEvent(DebouncedJoystick *joystick, int button = 0);
	virtual ~JoystickReleasedEvent();
	
	virtual bool Fired();
	
	virtual void Update();
	
	int GetButton();
	DebouncedJoystick* GetJoystick();
	
private:
	DebouncedJoystick *m_joystick;
	int m_button;
	int m_lastFiredButton;
};

#endif
