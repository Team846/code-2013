#include "Shooter.h"
#include "../ComponentData/ShooterData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

Shooter::Shooter()
: ComponentWithJaguar("Shooter", DriverStationConfig::DigitalIO::SHOOTER, true, RobotConfig::can::SHOOTER, "Shooter"),
  m_configSection("Shooter")
{ }

Shooter::~Shooter()
{
	// m_jaguar is deleted in base class
	//DELETE(m_jaguar);
}

void Shooter::onEnable()
{
	
}

void Shooter::onDisable()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Shooter::enabledPeriodic()
{
	
}

void Shooter::disabledPeriodic()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Shooter::Configure()
{
	m_dutyCycle = ConfigManager::Instance()->Get<float> (m_configSection, "speed", 0.3F);
}

void Shooter::Log()
{
	
}
