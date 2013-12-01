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

bool JoystickPressedEvent::Fired()
{
	if (m_button == 0)
	{
		for (int i = 1; i <= m_joystick->GetNumButtons(); i++)
		{
			if (m_joystick->IsButtonJustPressed(i))
			{
				m_lastFiredButton = i;
				return true;
			}
		}
	}
	else if (m_joystick->IsButtonJustPressed(m_button))
	{
		m_lastFiredButton = m_button;
		return true;
	}
	return false;
}

void JoystickPressedEvent::Update()
{
}

int JoystickPressedEvent::GetButton()
{
	return m_lastFiredButton;
}

DebouncedJoystick* JoystickPressedEvent::GetJoystick()
{
	return m_joystick;
}
