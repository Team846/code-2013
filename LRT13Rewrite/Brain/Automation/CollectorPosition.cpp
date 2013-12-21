#include "CollectorPosition.h"

CollectorPosition::CollectorPosition(bool down) :
	Automation("CollectorPosition")
{
	m_collector = new Pneumatics(ConfigPortMappings::Get("Solenoid/COLLECTOR_A"), ConfigPortMappings::Get("Solenoid/COLLECTOR_B"), "Collector");
}

void CollectorPosition::AllocateResources()
{
	AllocateResource(COLLECTOR_SLIDE);
}

Automation::Status CollectorPosition::Start(Event *trigger)
{
	return SUCCESS;
}

bool CollectorPosition::Run()
{
	if (m_down)
		m_collector->Set(Pneumatics::FORWARD);
	else
		m_collector->Set(Pneumatics::REVERSE);
	return true;
}

Automation::Status CollectorPosition::Abort(Event *trigger)
{
	return SUCCESS;
}
