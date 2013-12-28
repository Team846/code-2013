#include "Angle.h"

Angle::Angle(bool high) :
	Automation("Angle")
{
	m_angle = new Pneumatics(ConfigPortMappings::Get("Solenoid/SHOOTER_ANGLER_A"), ConfigPortMappings::Get("Solenoid/SHOOTER_ANGLER_B"), "Angle");
}

void Angle::AllocateResources()
{
	AllocateResource(ANGLE);
}

bool Angle::Start()
{
	return true;
}

bool Angle::Run()
{
	if (m_high)
		m_angle->Set(Pneumatics::FORWARD);
	else
		m_angle->Set(Pneumatics::REVERSE);
	return true;
}

bool Angle::Abort()
{
	return true;
}
