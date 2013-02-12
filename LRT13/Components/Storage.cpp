#include "Storage.h"
#include "../ComponentData/StorageData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

Storage::Storage()
: Component("Storage", DriverStationConfig::DigitalIns::STORAGE, true),
  m_configSection("Storage")
{
	m_jaguar = new AsyncCANJaguar(RobotConfig::CAN::STORAGE, "Storage");
	m_dutyCycle = 0.0;
}

Storage::~Storage()
{
	DELETE(m_jaguar);
}

void Storage::onEnable()
{
	
}

void Storage::onDisable()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Storage::enabledPeriodic()
{
	m_jaguar->SetDutyCycle(m_dutyCycle);
}

void Storage::disabledPeriodic()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Storage::Configure()
{
	m_dutyCycle = ConfigManager::Instance()->Get<float> (m_configSection, "speed", 0.3F);
}

void Storage::Log()
{
	
}
