#include "JoystickMovedEvent.h"
#include <Math.h>

JoystickMovedEvent::JoystickMovedEvent(DebouncedJoystick *joystick, int axis, float sensitivity)
{
	m_joystick = joystick;
	m_axis = axis;
	m_sensitivity = sensitivity;
	m_lastFiredAxis = axis;
}

JoystickMovedEvent::~JoystickMovedEvent()
{
}

bool JoystickMovedEvent::CheckCondition()
{
	if (m_axis == 0)
	{
		for (int i = 1; i <= m_joystick->GetNumAxes(); i++)
		{
			if (fabs(m_joystick->GetRawAxis(i)) >= m_sensitivity)
			{
				return true;
			}
		}
	}
	else if (fabs(m_joystick->GetRawAxis(m_axis)) >= m_sensitivity)
	{
		return true;
	}
	return false;
}

int JoystickMovedEvent::GetAxis()
{
	return m_lastFiredAxis;
}

DebouncedJoystick* JoystickMovedEvent::GetJoystick()
{
	return m_joystick;
}
