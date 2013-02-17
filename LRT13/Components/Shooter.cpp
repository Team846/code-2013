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
	m_pneumatics = Pneumatics::Instance();
	
	m_speed_front = 0;
	m_speed_back = 0;
	atSpeedCounter[FRONT] = 0;
	atSpeedCounter[BACK] = 0;
	
	atSpeed[FRONT] = false;
	atSpeed[BACK] = false;
	
	m_proximity = new DigitalInput(RobotConfig::Digital::PROXIMITY_B);
	frisbee_detected = false;

	m_dutyCycleFront = 0.0;
	m_dutyCycleBack = 0.0;
	
	m_overCurrentCounter = 0;
	m_underCurrentCounter = 0;
	
	requiredCycles = 0;
	acceptableSpeedError[FRONT] = 0;
	acceptableSpeedError[BACK] = 0;
	
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
	m_PIDs[FRONT].setSetpoint(m_componentData->shooterData->GetDesiredSpeed(FRONT));
	m_PIDs[BACK].setSetpoint(m_componentData->shooterData->GetDesiredSpeed(BACK));
	
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
	//m_speed_front = Util::Clamp<double>(m_speed_front, 0, m_max_speed * 1.3);
	m_speed_back = (m_enc_back->GetStopped()) ? 0.0 : (60.0 / 2.0 / m_enc_back->GetPeriod());
	//m_speed_back = Util::Clamp<double>(m_speed_back, 0, m_max_speed * 1.3);
	
	frisbee_detected = m_proximity->Get() == 0;
	// TODO: change shooter speed based on orientation
	
	CheckError(FRONT);
	CheckError(BACK);
	
	//TODO: write piston code when pneumatics code is fixed
	if(atSpeed[FRONT] && atSpeed[BACK])
	{
		if (m_componentData->shooterData->ShouldExtendLauncher())
		{
			m_pneumatics->setStorageExit(true);
		}
		else
		{
			m_pneumatics->setStorageExit(false);
		}
	}
	m_PIDs[FRONT].setInput(m_speed_front);
	m_PIDs[BACK].setInput(m_speed_back);
	m_jaguar_front->SetDutyCycle(m_PIDs[FRONT].update(1.0 / RobotConfig::LOOP_RATE));
	m_jaguar_back->SetDutyCycle(m_PIDs[BACK].update(1.0 / RobotConfig::LOOP_RATE));
}

void Shooter::disabledPeriodic()
{
	m_jaguar_front->SetDutyCycle(0.0F);
	m_jaguar_back->SetDutyCycle(0.0F);
}

void Shooter::Configure()
{
	ConfigManager * c = ConfigManager::Instance();
	m_max_speed = c->Get<double> (m_configSection, "maxSpeed", 5180);
	
	m_dutyCycleFront = c->Get<double> (m_configSection, "frontSpeed", 0.3F);
	m_dutyCycleBack = c->Get<double> (m_configSection, "backSpeed", 0.3F);
	
	//TODO: Change default values.
	requiredCycles = c->Get<int>(m_configSection, "requiredCycles", 9);
	acceptableSpeedError[FRONT] = c->Get<double>(m_configSection, "front_acceptableSpeedError", 0);
	acceptableSpeedError[BACK] = c->Get<double>(m_configSection, "back_acceptableSpeedError", 0);
	
}

void Shooter::Log()
{
	
}

void Shooter::CheckError(int roller) 
{
	
	if(m_PIDs[roller].getError() > acceptableSpeedError[roller]) 
	{
		atSpeedCounter[roller] = 0;
		atSpeed[roller] = false;
			
	}
	else 
	{
		atSpeedCounter[roller]++;
		if(atSpeedCounter[roller] > requiredCycles) {
			atSpeed[roller] = true;
		}
	}
	
}
