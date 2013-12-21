#include "TrapezoidProfile.h"
#include "math.h"
#include "../Utils/Util.h"

TrapezoidProfile::TrapezoidProfile(double maxV, double timeToMaxV)
{
	m_maxV = maxV;
	m_timeToMaxV = timeToMaxV;
	m_accel = m_maxV / m_timeToMaxV;
	m_setpoint = 0;
	m_velocity = 0;
}

void TrapezoidProfile::updateValues(double maxV, double timeToMaxV)
{
	m_maxV = maxV;
	m_timeToMaxV = timeToMaxV;
	m_accel = m_maxV / m_timeToMaxV;
	m_setpoint = 0;
	m_velocity = 0;
}

double TrapezoidProfile::Update(double currentTime)
{
	double timeElapsed = currentTime - m_startingTime;
	double accel = Util::Sign(m_setpoint) * m_accel;
	double maxV = Util::Sign(m_setpoint) * m_maxV;
	if (timeElapsed < m_timeToMax)
	{
		m_velocity = accel * timeElapsed;
		m_output += m_velocity * (currentTime - m_lastTime);
	}
	else if (timeElapsed < m_timeFromMax)
	{
		m_velocity = maxV;
		m_output += m_velocity * (currentTime - m_lastTime);
	}
	else if (timeElapsed < m_timeTotal)
	{
		m_velocity = (m_timeToMax * accel - accel * (timeElapsed - m_timeFromMax));
		m_output += m_velocity * (currentTime - m_lastTime);
	}
	else
	{
		m_velocity = 0;
		m_output = m_setpoint;
	}
	m_lastTime = currentTime;
	return m_output;
}

double TrapezoidProfile::getVelocity()
{
	return m_velocity;
}

double TrapezoidProfile::getOutput()
{
	return m_output;
}

void TrapezoidProfile::SetSetpoint(double setpoint, double time)
{
	m_setpoint = setpoint;
	m_output = 0.0;
	m_velocity = 0.0;
	m_startingTime = time;
	m_lastTime = time;
	double maxV = Util::Sign(setpoint) * m_maxV;
	double accel = Util::Sign(setpoint) * m_accel;
	m_timeToMax = maxV / accel; // Time spent accelerating
	double timeAtMax = (setpoint - 2 * (m_timeToMax * maxV - accel * m_timeToMax * m_timeToMax / 2)) / maxV; // Time spent at max velocity
	if (timeAtMax < 0)
	{
		m_timeToMax = Util::Sign(setpoint) * sqrt(2 * accel * m_setpoint / 2) / accel; // t = (sqrt(v^2+2ad)-v)/a
		timeAtMax = 0;
	}
	m_timeFromMax = m_timeToMax + timeAtMax; // Time to start decelerating
	m_timeTotal = m_timeFromMax + m_timeToMax; // Add deceleration
}
