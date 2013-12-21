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

double ProfiledPID::Update(double dt)
{
	double setpoint = m_profile->Update(m_timer.Get());
	PID::SetSetpoint(setpoint);
	printf("Motion Profile Setpoint: %f\n", setpoint);
	return PID::Update(dt);
}

void ProfiledPID::SetSetpoint(double setpoint)
{
	m_timer.Reset();
	m_timer.Start();
	m_setpoint = setpoint;
	m_profile->SetSetpoint(m_setpoint, m_timer.Get());
	PID::SetSetpoint(0.0);
}

void ProfiledPID::SetMotionProfile(MotionProfile *profile)
{
	m_profile = profile;
	PID::SetSetpoint(0.0);
}
