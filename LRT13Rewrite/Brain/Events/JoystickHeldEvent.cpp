#include "JoystickHeldEvent.h"

JoystickHeldEvent::JoystickHeldEvent(DebouncedJoystick *joystick, int button, int cycles)
{
	m_joystick = joystick;
	m_button = button;
	m_cycles = cycles;
	m_lastFiredButton = button;
	m_currentCycles = 0;
}

JoystickHeldEvent::~JoystickHeldEvent()
{
}

bool JoystickHeldEvent::CheckCondition()
{
	return m_currentCycles >= m_cycles;
}

void JoystickHeldEvent::Update()
{
	Event::Update();
	if (m_joystick->IsButtonDown(m_button))
	{
		m_currentCycles++;
	}
	else
	{
		m_currentCycles = 0;
	}
}

int JoystickHeldEvent::GetButton()
{
	return m_lastFiredButton;
}

DebouncedJoystick* JoystickHeldEvent::GetJoystick()
{
	return m_joystick;
}
