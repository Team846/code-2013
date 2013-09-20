#include "TrapezoidProfile.h"

TrapezoidProfile::TrapezoidProfile(double maxV, double timeToMaxV)
{
	m_maxV = maxV;
	m_timeToMaxV = timeToMaxV;
	m_accel = m_maxV / m_timeToMaxV;
	m_setpoint = 0;
}

void TrapezoidProfile::updateValues(double maxV, double timeToMaxV)
{
	m_maxV = maxV;
	m_timeToMaxV = timeToMaxV;
	m_accel = m_maxV / m_timeToMaxV;
	m_setpoint = 0;
}

double TrapezoidProfile::update(double currentTime)
{
	double timeElapsed = currentTime - m_startingTime;
	if (timeElapsed < m_timeToMax)
	{
		m_output += m_accel * timeElapsed * (currentTime - m_lastTime);
	}
	else if (timeElapsed < m_timeFromMax)
	{
		m_output += m_maxV * (currentTime - m_lastTime);
	}
	else if (timeElapsed < m_timeTotal)
	{
		m_output += (m_maxV - m_accel * (timeElapsed - m_timeFromMax)) * (currentTime - m_lastTime);
	}
	m_lastTime = currentTime;
	return m_output;
}

void TrapezoidProfile::setSetpoint(double setpoint, double time)
{
	m_setpoint = setpoint;
	m_output = 0.0;
	m_startingTime = time;
	m_lastTime = time;
	m_timeToMax = m_maxV / m_accel; // Time spent accelerating
	double timeAtMax = (setpoint - 2 * (m_timeToMax * m_maxV - m_accel * m_timeToMax * m_timeToMax / 2)) / m_maxV; // Time spent at max velocity
	m_timeFromMax = m_timeToMax + timeAtMax; // Time to start decelerating
	m_timeTotal = m_timeFromMax + m_timeToMax; // Add deceleration
}
