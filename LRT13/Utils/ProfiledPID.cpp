#include "ProfiledPID.h"
#include <cstdio>

ProfiledPID::ProfiledPID(MotionProfile *profile, double p_gain, double i_gain, double d_gain, double ff_gain,
		double i_decay, bool feedforward) :
	PID(p_gain, i_gain, d_gain, ff_gain, i_decay, feedforward)
{
	m_profile = profile;
}

ProfiledPID::ProfiledPID(MotionProfile *profile) :
	PID()
{
	m_profile = profile;
}

ProfiledPID::~ProfiledPID()
{
	
}

double ProfiledPID::update(double dt)
{
	PID::setSetpoint(m_profile->update(m_timer.Get()));
	printf("Motion Profile Setpoint: %f\n", m_profile->update(m_timer.Get()));
	return PID::update(dt);
}

void ProfiledPID::setSetpoint(double setpoint)
{
	m_timer.Reset();
	m_timer.Start();
	m_setpoint = setpoint;
	m_profile->setSetpoint(m_setpoint, m_timer.Get());
	PID::setSetpoint(0.0);
}

void ProfiledPID::setMotionProfile(MotionProfile *profile)
{
	m_profile = profile;
	PID::setSetpoint(0.0);
}
