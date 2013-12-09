#include "CollectorSlide.h"
#include "../Config/DriverStationConfig.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/ConfigRuntime.h"
#include "../Utils/AsyncPrinter.h"
#include "../RobotState.h"

CollectorSlide::CollectorSlide() :
	Configurable("collector")
{
	m_angle = new Pneumatics(ConfigPortMappings::Get("Solenoid/SHOOTER_ANGLER_A"), ConfigPortMappings::Get("Solenoid/SHOOTER_ANGLER_B"), "Angle");
	m_collector = new Pneumatics(ConfigPortMappings::Get("Solenoid/COLLECTOR_A"), ConfigPortMappings::Get("Solenoid/COLLECTOR_B"), "Collector");
	m_timer = 0;
	aborting = false;
}

CollectorSlide::~CollectorSlide()
{

}

Automation::Status CollectorSlide::Start(Event *trigger)
{
	aborting = false;
	m_timer = 0;
	return SUCCESS;
}

void CollectorSlide::AllocateResources()
{
	
}

bool CollectorSlide::Run()
{
	if (!aborting)
	{
		m_angle->Set(Pneumatics::REVERSE);
		m_collector->Set(Pneumatics::FORWARD);
		m_timer++;
		if (m_timer >= m_extendCycles)
		{
			m_angle->Set(Pneumatics::FORWARD);
		}
	}
	else
	{
		m_angle->Set(Pneumatics::REVERSE);
		m_timer++;
		if (m_timer >= m_retractCycles)
		{
			m_collector->Set(Pneumatics::REVERSE);
			return true;
		}
	}
	return false;
}

Automation::Status CollectorSlide::Abort(Event *trigger)
{
	if (!aborting)
	{
		aborting = true;
		m_timer = 0;
	}
	return CLEANING_UP;
}

void CollectorSlide::Configure()
{
	m_extendCycles = m_config->Get<int>(m_configSection, "cyclesBeforeExtend", 50);
	m_retractCycles = m_config->Get<int> (m_configSection, "cyclesBeforeRetract", 20);
}

