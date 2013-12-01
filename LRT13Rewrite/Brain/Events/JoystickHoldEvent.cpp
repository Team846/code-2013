#include "JoystickHoldEvent.h"

JoystickHoldEvent::JoystickHoldEvent(DebouncedJoystick *joystick, int button, int cycles)
{
	m_joystick = joystick;
	m_button = button;
	m_cycles = cycles;
	m_lastFiredButton = button;
	m_currentCycles = 0;
	m_fired = false;
}

JoystickHoldEvent::~JoystickHoldEvent()
{
}

bool JoystickHoldEvent::Fired()
{
	return m_fired;
}

void JoystickHoldEvent::Update()
{
	if (m_joystick->IsButtonDown(m_button))
	{
		m_lastFiredButton = m_button;
		m_currentCycles++;
	}
	else
	{
		m_currentCycles = 0;
	}
	if (m_currentCycles >= m_cycles)
	{
		m_fired = true;
		m_lastFiredButton = m_button;
	}
	else
	{
		m_fired = false;
	}
}

int JoystickHoldEvent::GetButton()
{
	return m_lastFiredButton;
}

DebouncedJoystick* JoystickHoldEvent::GetJoystick()
{
	return m_joystick;
}
