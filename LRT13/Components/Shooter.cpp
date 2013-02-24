#include "Shooter.h"
#include "../ComponentData/ShooterData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../ComponentData/ShooterData.h"

using namespace data;
using namespace data::shooter;

Shooter::Shooter() :
	Component("Shooter", DriverStationConfig::DigitalIns::SHOOTER, true),
			m_configSection("Shooter")
{
	m_jaguars[FRONT] = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_A,
			"ShooterFront");
	m_jaguars[BACK] = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_B,
			"ShooterBack");
	m_encs[FRONT] = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT_A);
	m_encs[FRONT]->Start();
	m_encs[BACK] = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT_B);
	m_encs[BACK]->Start();
	m_pneumatics = Pneumatics::Instance();

	atSpeedCounter[FRONT] = 0;
	atSpeedCounter[BACK] = 0;

	atSpeed[FRONT] = false;
	atSpeed[BACK] = false;

	m_proximity = new DigitalInput(RobotConfig::Digital::PROXIMITY_B);

	requiredCyclesAtSpeed = 0;
	acceptableSpeedError[FRONT] = 0;
	acceptableSpeedError[BACK] = 0;
	lastSpeed = 0;
	m_speeds[FRONT] = 0;
	m_speeds[BACK] = 0;
	
	Configure();

}

Shooter::~Shooter()
{
	DELETE(m_jaguars[FRONT]);
	DELETE(m_jaguars[BACK]);
}

void Shooter::onEnable()
{

}

void Shooter::onDisable()
{
	m_jaguars[FRONT]->SetDutyCycle(0.0F);
	m_jaguars[BACK]->SetDutyCycle(0.0F);
}

void Shooter::enabledPeriodic()
{	
	ManageShooterWheel(FRONT);
	ManageShooterWheel(BACK);
	
	double frisbee_detected = m_proximity->Get() == 0;
	if(atSpeed[FRONT] && atSpeed[BACK])
	{
		if (m_componentData->shooterData->GetShooterSetting() == AUTO)
		{
			if(m_componentData->shooterData->GetNumFrisbeesInStorage() > 0)
			{
				bool m_isExtended = m_pneumatics->GetStorageExitState();
				if (m_isExtended)
				{
					//if the speed of the first shooter wheel drops a ton. (Defined by a threshold read fromconfig)
					if(m_speeds[FRONT] < (lastSpeed - frisbeeDetectionThreshold))
					{	
						m_pneumatics->setStorageExit(RETRACTED);

					}
					m_isExtended = false;
				}
				else
				{
					if (frisbee_detected)
					{
						//extend
						m_pneumatics->setStorageExit(EXTENDED);
						m_isExtended = true;
					}
				}
			}
			else
			{
				m_componentData->shooterData->SetShooterSetting(OFF);
			}
		} else if(m_componentData->shooterData->GetShooterSetting() == ONCE)
		{
			m_pneumatics->setStorageExit(RETRACTED);
			bool m_isExtended = false;

			if(!m_isExtended)
			{
				if(frisbee_detected)
				{
					m_pneumatics->setStorageExit(EXTENDED);
					m_isExtended = true;
				}
			} 
			else
			{
				m_pneumatics->setStorageExit(RETRACTED);
				m_componentData->shooterData->SetShooterSetting(OFF);
			}
		} 
		else
		{
			m_pneumatics->setStorageExit(RETRACTED);
		}
	}
	lastSpeed = m_speeds[FRONT];
	
	//m_speed_back = Util::Clamp<double>(m_speed_back, 0, m_max_speed * 1.3);

	// TODO: change shooter speed based on orientation
}

void Shooter::ManageShooterWheel(int roller)
{
	m_speeds[roller] = (double) (m_encs[roller]->GetStopped()) ? 0.0 : (60.0 / 2.0 / m_encs[roller]->GetPeriod());
	
	m_PIDs[roller].setInput(m_speeds[roller]/m_max_speed[roller]);
	m_PIDs[roller].setSetpoint(m_componentData->shooterData->GetDesiredSpeed((Roller)roller));
	
	double out = m_PIDs[roller].update(1.0 / RobotConfig::LOOP_RATE); //out is a normalized voltage
	
	double maxOut = m_speeds[roller] + m_maxNormalizedCurrent;
	
	//now lets limit the current
	out = Util::Min<double>(out, maxOut);
	out /= (DriverStation::GetInstance()->GetBatteryVoltage() / RobotConfig::MAX_VOLTAGE);
	
	m_jaguars[roller]->SetDutyCycle(out);
	
	if(m_PIDs[roller].getError() < acceptableSpeedError[roller])
	{
		atSpeedCounter[roller]++;
		atSpeed[roller] = atSpeedCounter[roller] > requiredCyclesAtSpeed;
	}
	else
	{
		atSpeedCounter[roller] = 0;
		atSpeed[roller] = false;
	}
	
}

void Shooter::disabledPeriodic()
{
	m_jaguars[FRONT]->SetDutyCycle(0.0F);
	m_jaguars[BACK]->SetDutyCycle(0.0F);
}

void Shooter::Configure()
{
	ConfigManager * c = ConfigManager::Instance();
	m_max_speed[FRONT] = c->Get<double> (m_configSection, "front_maxSpeed",
			5180);
	m_max_speed[BACK] = c->Get<double> (m_configSection, "back_maxSpeed", 5180);

	//TODO: Change default values.
	requiredCyclesAtSpeed = c->Get<int> (m_configSection, "requiredCycles", 9);
	acceptableSpeedError[FRONT] = c->Get<double> (m_configSection,
			"front_acceptableSpeedError", 0);
	acceptableSpeedError[BACK] = c->Get<double> (m_configSection,
			"back_acceptableSpeedError", 0);
	
	frisbeeDetectionThreshold = c->Get<double> (m_configSection, "shooterSpeedDrop", 0);
	
}

void Shooter::Log()
{

}
