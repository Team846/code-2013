#include "CollectorRollers.h"
#include "../Config/ConfigRuntime.h"
#include "../Config/ConfigPortMappings.h"
#include "../Config/DriverStationConfig.h"
#include "../Actuators/AsyncCANJaguar.h"

CollectorRollers::CollectorRollers() :
	Component("CollectorRollers", DriverStationConfig::DigitalIns::COLLECTOR, true),
	Configurable("CollectorRollers")
{
	m_jaguar = new LRTJaguar(ConfigPortMappings::Get("PWM/COLLECTOR"), "CollectorRollers");
	m_collectorRollersData = CollectorRollersData::Get();
}

CollectorRollers::~CollectorRollers()
{

}

void CollectorRollers::UpdateEnabled()
{
	if (m_collectorRollersData->IsRunning())
	{
		if (m_collectorRollersData->GetDirection() == CollectorRollersData::FORWARD)
			m_jaguar->SetDutyCycle(m_speed);
		else
			m_jaguar->SetDutyCycle(-m_speed);
	}
	else
		m_jaguar->SetDutyCycle(0.0);
}

void CollectorRollers::UpdateDisabled()
{
	m_jaguar->SetDutyCycle(0.0);
	m_jaguar->SetDutyCycle(0.0);
}

void CollectorRollers::OnEnabled()
{
}

void CollectorRollers::OnDisabled()
{
	m_jaguar->SetDutyCycle(0.0);
	m_jaguar->SetDutyCycle(0.0);
}

void CollectorRollers::Configure()
{
	m_speed = m_config->Get<float> (m_configSection, "speed", 1.0);
}
