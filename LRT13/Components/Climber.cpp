#include "Climber.h"
#include "../ComponentData/ClimberData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

Climber::Climber()
: Component("Climber", DriverStationConfig::DigitalIns::CLIMBER, true)
,m_configSection("climber")
{
	
}

Climber::~Climber()
{
	// m_jaguar is deleted in base class
	//DELETE(m_jaguar);
}

void Climber::onEnable()
{
	
}

void Climber::onDisable()
{
}

void Climber::enabledPeriodic()
{
}

void Climber::disabledPeriodic()
{
}

void Climber::Configure()
{
}

void Climber::Log()
{
	
}
