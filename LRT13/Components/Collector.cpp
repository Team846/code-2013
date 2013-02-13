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
	m_proximityA = new DigitalInput(RobotConfig::Digital::PROXIMITY_A);
	m_proximityB = new DigitalInput(RobotConfig::Digital::PROXIMITY_B);
	m_dutyCycle = 0.0;
	m_upCount = 0;
	m_downCount = 0;
	m_errCount = 0;
	m_errors = 0;
	m_samplesThreshold = 0;
}

Collector::~Collector()
{
	DELETE(m_proximityA);
	DELETE(m_proximityB);
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
	
	// Increment Orientation and Error Counters
	if (m_proximityA->Get() == 1 && m_proximityB->Get() == 0)
	{
		m_upCount++;
	}
	else
	{
		m_upCount = 0;
	}
	if (m_proximityA->Get() == 0 && m_proximityB->Get() == 1)
	{
		m_downCount++;
	}
	else
	{
		m_downCount = 0;
	}
	if (m_proximityA->Get() == 1 && m_proximityB->Get() == 1)
	{
		m_errCount++;
	}
	else
	{
		m_errCount = 0;
	}
	
	// Frisbees aren't flush against each other
	if (m_upCount == m_samplesThreshold)
	{
#warning FIXME OR ELSE I WILL ANNOY YOU
//		RobotData::IncrementFrisbeeCounter(RobotData::UP);
	}
	if (m_downCount == m_samplesThreshold)
	{
//		RobotData::IncrementFrisbeeCounter(RobotData::DOWN);	
	}
	if (m_errCount == m_samplesThreshold)
	{
		m_errors++;
	}
}

void Collector::disabledPeriodic()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Collector::Configure()
{
	m_dutyCycle = ConfigManager::Instance()->Get<float> (m_configSection, "speed", 0.3F);
	m_samplesThreshold = ConfigManager::Instance()->Get<int> (m_configSection, "samplesThreshold", 4);
}

void Collector::Log()
{
	// Log m_errors
}
