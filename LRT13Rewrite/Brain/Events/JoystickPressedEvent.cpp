#include "JoystickPressedEvent.h"

JoystickPressedEvent::JoystickPressedEvent(DebouncedJoystick *joystick, int button)
{
	m_joystick = joystick;
	m_button = button;
	m_lastFiredButton = button;
}

JoystickPressedEvent::~JoystickPressedEvent()
{
}

bool JoystickPressedEvent::CheckCondition()
{
	if (m_button == 0)
	{
		for (int i = 1; i <= m_joystick->GetNumButtons(); i++)
		{
			if (m_joystick->IsButtonDown(i))
			{
				return true;
			}
		}
	}
	else if (m_joystick->IsButtonDown(m_button))
	{
		return true;
	}
	return false;
}

int JoystickPressedEvent::GetButton()
{
	return m_lastFiredButton;
}

DebouncedJoystick* JoystickPressedEvent::GetJoystick()
{
	return m_joystick;
}
