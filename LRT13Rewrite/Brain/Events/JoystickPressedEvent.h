#ifndef JOYSTICK_PRESSED_EVENT_H_
#define JOYSTICK_PRESSED_EVENT_H_

#include "Event.h"
#include "../../DriverStation/DebouncedJoystick.h"

/*!
 * @brief Event that fires when a joystick button is pressed.
 */
class JoystickPressedEvent : public Event
{
public:
	JoystickPressedEvent(DebouncedJoystick *joystick, int button = 0);
	virtual ~JoystickPressedEvent();
	
	virtual bool CheckCondition();

	int GetButton();
	DebouncedJoystick* GetJoystick();
	
private:
	DebouncedJoystick *m_joystick;
	int m_button;
	int m_lastFiredButton;
};

#endif
