#include "JoystickPressedEvent.h"

JoystickPressedEvent::JoystickPressedEvent(DebouncedJoystick *joystick, int button)
{
	m_joystick = joystick;
	m_button = button;
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
				return true;
		}
	}
	else
	{
		return m_joystick->IsButtonJustPressed(m_button);
	}
	return false;
}

void JoystickPressedEvent::Update()
{
}

