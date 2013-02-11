#include "Shooter.h"
#include "../ComponentData/ShooterData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../ComponentData/ShooterData.h"

using namespace data;
using namespace data::shooter;

Shooter::Shooter()
: Component("Shooter", DriverStationConfig::DigitalIns::SHOOTER, true),
  m_configSection("Shooter")   
{ 
	m_jaguar = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER, "Shooter");
	m_enc = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT);
	
	m_shooterConfiguration = data::shooter::SLOW;
	wrongSpeedCounter = 0;
}

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
	
	m_speed = (m_enc->GetStopped()) ? 0.0 : (60.0 / 2.0 / m_enc->GetPeriod());
	m_speed = Util::Clamp<double>(m_speed, 0, m_max_speed * 1.3);
	
	if(m_speed < ShooterData().speed) 
	{
		wrongSpeedCounter++;
		if(wrongSpeedCounter > 9) 
		{	
			//FIX IT
		}
	} else
	{
		wrongSpeedCounter = 0;
	}
	
	
	
}

void Shooter::disabledPeriodic()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Shooter::Configure()
{
	m_dutyCycle = ConfigManager::Instance()->Get<float> (m_configSection, "speed", 0.3F);

	ConfigManager * c = ConfigManager::Instance();
	m_max_speed = c->Get<double> (m_configSection, "maxSpeed", 5180);
	
}

void Shooter::Log()
{
	
}
