#ifndef JOYSTICK_MOVED_EVENT_H_
#define JOYSTICK_MOVED_EVENT_H_

#include "Event.h"
#include "../../DriverStation/DebouncedJoystick.h"

/*!
 * @brief Event that fires when a joystick axis is moved.
 */
class JoystickMovedEvent : public Event
{
public:
	JoystickMovedEvent(DebouncedJoystick *joystick, int axis = 0, float sensitivity = 0.02);
	virtual ~JoystickMovedEvent();
	
	virtual bool CheckCondition();
	
	int GetAxis();
	DebouncedJoystick* GetJoystick();
	
private:
	DebouncedJoystick *m_joystick;
	int m_axis;
	float m_sensitivity;
	int m_lastFiredAxis;
};

#endif
