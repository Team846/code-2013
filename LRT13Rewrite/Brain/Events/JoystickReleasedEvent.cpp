#include "JoystickReleasedEvent.h"

JoystickReleasedEvent::JoystickReleasedEvent(DebouncedJoystick *joystick, int button)
{
	m_joystick = joystick;
	m_button = button;
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
				return true;
		}
	}
	else
	{
		return m_joystick->IsButtonJustReleased(m_button);
	}
	return false;
}

void JoystickReleasedEvent::Update()
{
}

