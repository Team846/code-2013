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
	m_dutyCycle = 0.0;
	m_upCount = 0;
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
	m_jaguar->SetDutyCycle(m_dutyCycle);

	if (m_proximity->Get() == 0)
	{
		m_upCount++;
	}
	else
	{
		m_upCount = 0;
	}
	
	// Frisbees aren't flush against each other
	if (m_upCount == m_samplesThreshold)
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
