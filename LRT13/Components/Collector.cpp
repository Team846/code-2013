#include "Collector.h"
#include "../ComponentData/CollectorData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

Collector::Collector()
: ComponentWithJaguar("Collector", DriverStationConfig::DigitalIO::COLLECTOR, true, RobotConfig::can::COLLECTOR, "Collector"),
  m_configSection("collector")
{ }

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

void Collector::whenEnabled()
{
	
}

void Collector::whenDisabled()
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
