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
	front_atSpeedCounter = 0;
	back_atSpeedCounter = 0;
	
	front_atSpeed = false;
	back_atSpeed = false;
	
	m_roller_jaguar = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_ROLLER, "Shooter Roller");
	m_dutyCycle = 0.0;
	
	m_roller_jaguar->addCollectionFlags(AsyncCANJaguar::OUTCURR);
	
	m_overCurrentCounter = 0;
	m_underCurrentCounter = 0;
}

Shooter::~Shooter()
{
	DELETE(m_jaguar_front);
	DELETE(m_jaguar_back);
	DELETE(m_roller_jaguar);
}

void Shooter::onEnable()
{
	
}

void Shooter::onDisable()
{
	m_jaguar_front->SetDutyCycle(0.0F);
	m_jaguar_back->SetDutyCycle(0.0F);
	
	m_roller_jaguar->SetDutyCycle(0.0F);
}

void Shooter::enabledPeriodic()
{
	m_roller_jaguar->SetDutyCycle(m_dutyCycle);
	
	/*
	if(m_roller_jaguar->GetOutputCurrent() > m_normalCurrent)
	{
		if(m_overCurrentCounter - m_underCurrentCounter < m_maxCounterDifference)
			++m_overCurrentCounter;
	}
	else
	{
		if(m_underCurrentCounter - m_overCurrentCounter < m_maxCounterDifference)
			++m_underCurrentCounter;
	}*/
	
	
	
	m_speed_front = (m_enc_front->GetStopped()) ? 0.0 : (60.0 / 2.0 / m_enc_front->GetPeriod());
	m_speed_front = Util::Clamp<double>(m_speed_front, 0, m_max_speed * 1.3);
	m_speed_back = (m_enc_back->GetStopped()) ? 0.0 : (60.0 / 2.0 / m_enc_back->GetPeriod());
	m_speed_back = Util::Clamp<double>(m_speed_back, 0, m_max_speed * 1.3);
	
	if(fabs(m_speed_front - m_componentData->shooterData->GetDesiredSpeed(FRONT)) > 
		m_componentData->shooterData->GetAcceptableSpeedError(FRONT)) 
	{
		front_atSpeedCounter = 0;
		front_atSpeed = false;
		
	} else {
		front_atSpeedCounter++;
		front_atSpeed = true;
	}	
	
	if(fabs(m_speed_back - m_componentData->shooterData->GetDesiredSpeed(BACK)) > 
		m_componentData->shooterData->GetAcceptableSpeedError(BACK)) 
	{
		
		back_atSpeedCounter = 0;
		back_atSpeed = false;
		
	} else {
		back_atSpeedCounter++;
		back_atSpeed = true;
	}
	
	if(!front_atSpeed || !back_atSpeed)
	{
		//Don't shoot
	}
	
}

bool Shooter::IsOverCurrent()
{
	//return m_overCurrentCounter > m_underCurrentCounter;
	return m_roller_jaguar->GetOutputCurrent() > m_roller_currentThreshold;
}

void Shooter::disabledPeriodic()
{
	m_jaguar_front->SetDutyCycle(0.0F);
	m_jaguar_back->SetDutyCycle(0.0F);
	
	m_roller_jaguar->SetDutyCycle(0.0F);
}

void Shooter::Configure()
{
	ConfigManager * c = ConfigManager::Instance();
	m_max_speed = c->Get<double> (m_configSection, "maxSpeed", 5180);
	
	m_dutyCycle = ConfigManager::Instance()->Get<double> (m_configSection, "speed", 0.3F);
	
#warning change me
	m_roller_currentThreshold = ConfigManager::Instance()->Get<double>(m_configSection, "normal_current", 10);
	
	m_maxCounterDifference = ConfigManager::Instance()->Get<double>(m_configSection, "max_counter_difference", 2);
}

void Shooter::Log()
{
	
}
