#include "JoystickReleasedEvent.h"

JoystickReleasedEvent::JoystickReleasedEvent(DebouncedJoystick *joystick, int button)
{
	m_joystick = joystick;
	m_button = button;
	m_lastFiredButton = button;
}

JoystickReleasedEvent::~JoystickReleasedEvent()
{
}

bool JoystickReleasedEvent::Fired()
{
	if (m_button == 0)
	{
		for (int i = 1; i <= m_joystick->GetNumButtons(); i++)
		{
			if (m_joystick->IsButtonJustReleased(i))
			{
				m_lastFiredButton = i;
				return true;
			}
		}
	}
	else if (m_joystick->IsButtonJustReleased(m_button))
	{
		m_lastFiredButton = m_button;
		return true;
	}
	return false;
}

void JoystickReleasedEvent::Update()
{
}

int JoystickReleasedEvent::GetButton()
{
	return m_lastFiredButton;
}

DebouncedJoystick* JoystickReleasedEvent::GetJoystick()
{
	return m_joystick;
}
