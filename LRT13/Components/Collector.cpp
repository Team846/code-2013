#include "Collector.h"
#include "../ComponentData/CollectorData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

Collector::Collector()
: ComponentWithJaguar("Collector", DriverStationConfig::DigitalIns::COLLECTOR, true, RobotConfig::CAN::COLLECTOR, "Collector"),
  m_configSection("collector")
{
	
}

Collector::~Collector()
{
	// m_jaguar is deleted in base class
	//DELETE(m_jaguar);
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
}

void Collector::disabledPeriodic()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Collector::Configure()
{
	m_dutyCycle = ConfigManager::Instance()->Get<float> (m_configSection, "speed", 0.3F);
}

void Collector::Log()
{
	
}
