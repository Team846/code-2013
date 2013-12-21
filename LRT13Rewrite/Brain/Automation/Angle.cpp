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

Automation::Status Angle::Start(Event *trigger)
{
	return SUCCESS;
}

bool Angle::Run()
{
	if (m_high)
		m_angle->Set(Pneumatics::FORWARD);
	else
		m_angle->Set(Pneumatics::REVERSE);
	return true;
}

Automation::Status Angle::Abort(Event *trigger)
{
	return SUCCESS;
}
