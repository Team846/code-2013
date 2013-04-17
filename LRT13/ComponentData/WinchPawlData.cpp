#include "WinchPawlData.h"

using namespace data;
using namespace data::climber;

WinchPawlData::WinchPawlData()
{
	m_current = 0.0;
	m_dutyCycle = 0.0f;
}

void WinchPawlData::setDutyCyle(float f)
{
	m_dutyCycle = f;
}

float WinchPawlData::getDutyCycle()
{
	return m_dutyCycle;
}

void WinchPawlData::updateMotorCurrent(double d)
{
	m_current = d;
}

double WinchPawlData::getMotorCurrent()
{
	return m_current;
}

bool WinchPawlData::isWinchPawlTimedOut()
{
	return m_isWinchPawlTimedOut;
}

void WinchPawlData::setWinchPawlTimedOut(bool b)
{
	m_isWinchPawlTimedOut = b;
}
