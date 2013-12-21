#ifndef JOYSTICK_HELD_EVENT_H_
#define JOYSTICK_HELD_EVENT_H_

#include "Event.h"
#include "../../DriverStation/DebouncedJoystick.h"

/*!
 * @brief Event that fires when a joystick button is held.
 */
class JoystickHeldEvent : public Event
{
public:
	JoystickHeldEvent(DebouncedJoystick *joystick, int button, int cycles);
	virtual ~JoystickHeldEvent();
	
	virtual bool CheckCondition();
	virtual void Update();

	int GetButton();
	DebouncedJoystick* GetJoystick();
	
private:
	DebouncedJoystick *m_joystick;
	int m_button;
	int m_cycles;
	int m_lastFiredButton;
	int m_currentCycles;
};

#endif
