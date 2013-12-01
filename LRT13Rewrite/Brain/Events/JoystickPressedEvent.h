#ifndef JOYSTICK_PRESSED_EVENT_H_
#define JOYSTICK_PRESSED_EVENT_H_

#include "Event.h"
#include "../../DriverStation/DebouncedJoystick.h"

class JoystickPressedEvent : public Event
{
public:
	JoystickPressedEvent(DebouncedJoystick *joystick, int button = 0);
	virtual ~JoystickPressedEvent();
	
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
