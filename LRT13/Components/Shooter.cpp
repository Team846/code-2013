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
	m_jaguar_front = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_A, "ShooterFront");
	m_jaguar_back = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_B, "ShooterBack");
	m_enc_front = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT_A);
	m_enc_back = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT_B);
	
	m_speed_front = 0;
	m_speed_back = 0;
	wrongSpeedCounter = 0;
}

Shooter::~Shooter()
{
	DELETE(m_jaguar_front);
	DELETE(m_jaguar_back);
}

void Shooter::onEnable()
{
	
}

void Shooter::onDisable()
{
	m_jaguar_front->SetDutyCycle(0.0F);
	m_jaguar_back->SetDutyCycle(0.0F);
}

void Shooter::enabledPeriodic()
{
	m_speed_front = (m_enc_front->GetStopped()) ? 0.0 : (60.0 / 2.0 / m_enc_front->GetPeriod());
	m_speed_front = Util::Clamp<double>(m_speed_front, 0, m_max_speed * 1.3);
	m_speed_back = (m_enc_back->GetStopped()) ? 0.0 : (60.0 / 2.0 / m_enc_back->GetPeriod());
	m_speed_back = Util::Clamp<double>(m_speed_back, 0, m_max_speed * 1.3);
	
	if(m_speed_front < m_componentData->shooterData->GetDesiredSpeed(FRONT)) 
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
	m_jaguar_front->SetDutyCycle(0.0F);
	m_jaguar_back->SetDutyCycle(0.0F);
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
