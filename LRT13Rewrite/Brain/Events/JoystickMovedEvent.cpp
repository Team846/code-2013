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

bool JoystickMovedEvent::Fired()
{
	if (m_axis == 0)
	{
		for (int i = 1; i <= m_joystick->GetNumAxes(); i++)
		{
			if (m_joystick->GetLastAxis(i) && fabs(m_joystick->GetRawAxis(i)) >= m_sensitivity)
			{
				m_lastFiredAxis = i;
				return true;
			}
		}
	}
	else if (m_joystick->GetLastAxis(m_axis) && fabs(m_joystick->GetRawAxis(m_axis)) >= m_sensitivity)
	{
		m_lastFiredAxis = m_axis;
		return true;
	}
	return false;
}

void JoystickMovedEvent::Update()
{
	
}

int JoystickMovedEvent::GetAxis()
{
	return m_lastFiredAxis;
}

DebouncedJoystick* JoystickMovedEvent::GetJoystick()
{
	return m_joystick;
}
