#ifndef JOYSTICK_HOLD_EVENT_H_
#define JOYSTICK_HOLD_EVENT_H_

#include "Event.h"
#include "../../DriverStation/DebouncedJoystick.h"

class JoystickHoldEvent : public Event
{
public:
	JoystickHoldEvent(DebouncedJoystick *joystick, int button, int cycles);
	virtual ~JoystickHoldEvent();
	
	virtual bool Fired();
	
	virtual void Update();

	int GetButton();
	DebouncedJoystick* GetJoystick();
	
private:
	DebouncedJoystick *m_joystick;
	int m_button;
	int m_cycles;
	int m_lastFiredButton;
	int m_currentCycles;
	int m_fired;
};

#endif
