#include "Collector.h"
#include "../ComponentData/CollectorData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

Collector::Collector()
: Component("Collector", DriverStationConfig::DigitalIns::COLLECTOR, true),
  m_configSection("collector")
{
	m_jaguar = new AsyncCANJaguar(RobotConfig::CAN::COLLECTOR, "Collector");
	m_proximity = new DigitalInput(RobotConfig::Digital::PROXIMITY_A);
	m_pneumatics = Pneumatics::Instance();
	m_dutyCycle = 0.0;
	m_count = 0;
	m_samplesThreshold = 0;
}

Collector::~Collector()
{
	DELETE(m_proximity);
	DELETE(m_jaguar);
}

void Collector::onEnable()
{
	
}

void Collector::onDisable()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Collector::enabledPeriodic()
{
	if (m_componentData->collectorData->ShouldRunRollers())
	{
		m_jaguar->SetDutyCycle(m_dutyCycle);
	}
	
	if (m_componentData->collectorData->IsDown())
	{
		m_pneumatics->setCollector(true);
	}
	else
	{
		m_pneumatics->setCollector(false);
	}
	
	if (m_proximity->Get() == 0)
	{
		m_count++;
	}
	else
	{
		m_count = 0;
	}
	
	// Frisbees aren't flush against each other
	if (m_count == m_samplesThreshold)
	{
		RobotData::IncrementFrisbeeCounter();
	}
}

void Collector::disabledPeriodic()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Collector::Configure()
{
	m_dutyCycle = ConfigManager::Instance()->Get<float> (m_configSection, "speed", 0.5F);
	m_samplesThreshold = ConfigManager::Instance()->Get<int> (m_configSection, "samplesThreshold", 2);
}

void Collector::Log()
{
	
}
