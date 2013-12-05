#include "DebouncedJoystick.h"
#include <cstdio>
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"

DebouncedJoystick::DebouncedJoystick(UINT32 port, int nBtns, int nAxes) :
	Joystick(port),
	Loggable("Joystick" + Util::ToString(port))
{
	m_num_buttons = nBtns;
	m_num_axes = nAxes;
	// N.B. indexes are 1-based
	wasPressed = new bool[nBtns + 1];
	isPressed = new bool[nBtns + 1];
	axisPrevValue = new double[nAxes + 1];
	axisValue = new double[nAxes + 1];
	m_port = port;
	Init();
}

DebouncedJoystick::~DebouncedJoystick()
{
	DELETE(wasPressed);
	DELETE(isPressed);
	DELETE(axisPrevValue);
	DELETE(axisValue);
}

bool DebouncedJoystick::ButtonInBounds(int button)
{
	if (button <= 0 || button > m_num_buttons)
	{
		AsyncPrinter::Printf("[!]DebouncedJoystick: Button %d out of bounds!\n", button);
		return false;
	}
	return true;
}

bool DebouncedJoystick::AxisInBounds(int axis)
{
	if (axis <= 0 || axis > m_num_axes)
	{
		AsyncPrinter::Printf("[!]DebouncedJoystick: Axis %d out of bounds!\n", axis);
		return false;
	}
	return true;
}

void DebouncedJoystick::Init()
{
	for (int i = 1; i <= m_num_buttons; ++i)
	{
		wasPressed[i] = isPressed[i] = false;
	}

	for (int i = 1; i <= m_num_axes; ++i)
	{
		axisPrevValue[i] = axisValue[i] = 0;
	}
}

void DebouncedJoystick::Update()
{
	for (int i = 1; i <= m_num_buttons; ++i)
	{
		wasPressed[i] = isPressed[i];
		isPressed[i] = GetRawButton(i);
	}

	for (int i = 1; i <= m_num_axes; ++i)
	{
		axisPrevValue[i] = axisValue[i];
		axisValue[i] = GetRawAxis(i);
	}
}

double DebouncedJoystick::GetRawAxisDelta(int axis)
{
	// return positive when stick is pushed forward
	if (!AxisInBounds(axis))
		return false;
	return axisPrevValue[axis] - axisValue[axis];
}

double DebouncedJoystick::GetLastAxis(int axis)
{
	if (!AxisInBounds(axis))
		return false;
	return axisPrevValue[axis];
}

bool DebouncedJoystick::IsButtonJustPressed(int button)
{
	if (!ButtonInBounds(button))
		return false;
	return isPressed[button] && !wasPressed[button];
}

bool DebouncedJoystick::IsButtonJustReleased(int button)
{
	if (!ButtonInBounds(button))
		return false;
	return !isPressed[button] && wasPressed[button];
}

bool DebouncedJoystick::IsButtonDown(int button)
{
	if (!ButtonInBounds(button))
		return false;
	return isPressed[button];
}

bool DebouncedJoystick::WasButtonDown(int button)
{
	if (!ButtonInBounds(button))
		return false;
	return wasPressed[button];
}

bool DebouncedJoystick::IsHatSwitchJustPressed(int axis, int direction)
{
	return !WasHatSwitchDown(axis, direction) && IsHatSwitchDown(axis,
			direction);
}

bool DebouncedJoystick::IsHatSwitchJustReleased(int axis, int direction)
{
	return WasHatSwitchDown(axis, direction) && !IsHatSwitchDown(axis,
			direction);
}

bool DebouncedJoystick::WasHatSwitchDown(int axis, int direction)
{
	if (!AxisInBounds(axis))
		return false;
	return (axisPrevValue[axis] * direction > 0.5);
}

bool DebouncedJoystick::IsHatSwitchDown(int axis, int direction)
{
	if (!AxisInBounds(axis))
		return false;
	return (axisValue[axis] * direction > 0.5);
}

int DebouncedJoystick::GetNumButtons()
{
	return m_num_buttons;
}

int DebouncedJoystick::GetNumAxes()
{
	return m_num_axes;
}

UINT32 DebouncedJoystick::GetPort()
{
	return m_port;
}

void DebouncedJoystick::Log()
{
	LogToFile(&isPressed[1], m_num_buttons, "Buttons");
	LogToFile(&axisValue[1], m_num_axes, "Axes");
}
