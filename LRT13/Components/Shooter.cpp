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
	m_jaguar[FRONT] = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_A,
			"ShooterFront");
	m_jaguar[BACK] = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_B,
			"ShooterBack");
	m_enc_front = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT_A);
	m_enc_front->Start();
	m_enc_back = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT_B);
	m_enc_back->Start();
	m_pneumatics = Pneumatics::Instance();

	m_speed[FRONT] = 0;
	m_speed[BACK] = 0;
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

	maxDeltaDutyCycle = .75;
	m_duty_cycle_delta = .25;
	
	Configure();

}

Shooter::~Shooter()
{
	DELETE(m_jaguar[FRONT]);
	DELETE(m_jaguar[BACK]);
}

void Shooter::onEnable()
{

}

void Shooter::onDisable()
{
	m_jaguar[FRONT]->SetDutyCycle(0.0F);
	m_jaguar[BACK]->SetDutyCycle(0.0F);
}

void Shooter::enabledPeriodic()
{
	SetSetpoint(FRONT);
	SetSetpoint(BACK);

	m_speed[FRONT] = GetSpeed(m_enc_front);
	m_speed[BACK] = GetSpeed(m_enc_back);
	//m_speed_back = Util::Clamp<double>(m_speed_back, 0, m_max_speed * 1.3);

	frisbee_detected = m_proximity->Get() == 0;
	// TODO: change shooter speed based on orientation

	CheckError(FRONT);
	CheckError(BACK);

	//TODO: write autofire code
	if (atSpeed[FRONT] && atSpeed[BACK])
	{
		if (m_componentData->shooterData->ShouldLauncherBeHigh()) // Don't shoot
		{
			m_pneumatics->setStorageExit(true);
		}
		else // Shoot
		{
			m_pneumatics->setStorageExit(false);
			if (frisbee_detected)
			{
				m_componentData->shooterData->DecrementFrisbeeCounter();
				RobotData::DecrementFrisbeeCounter();
			}
		}
	}
	
	system_voltage = DriverStation::GetInstance()->GetBatteryVoltage();
	LimitCurrent(FRONT);
	LimitCurrent(BACK);
	
}

void Shooter::SetSetpoint(int roller)
{
	m_PIDs[roller].setSetpoint(m_componentData->shooterData->GetDesiredSpeed((Roller)roller));
}

double Shooter::GetSpeed(Counter* y)
{
	return (double)(y->GetStopped()) ? 0.0 : (60.0 / 2.0 / y->GetPeriod());
}

void Shooter::disabledPeriodic()
{
	m_jaguar[FRONT]->SetDutyCycle(0.0F);
	m_jaguar[BACK]->SetDutyCycle(0.0F);
}

void Shooter::Configure()
{
	ConfigManager * c = ConfigManager::Instance();
	m_max_speed[FRONT] = c->Get<double> (m_configSection, "front_maxSpeed",
			5180);
	m_max_speed[BACK] = c->Get<double> (m_configSection, "back_maxSpeed", 5180);

	m_dutyCycleFront = c->Get<double> (m_configSection, "frontSpeed", 0.3F);
	m_dutyCycleBack = c->Get<double> (m_configSection, "backSpeed", 0.3F);

	//TODO: Change default values.
	requiredCycles = c->Get<int> (m_configSection, "requiredCycles", 9);
	acceptableSpeedError[FRONT] = c->Get<double> (m_configSection,
			"front_acceptableSpeedError", 0);
	acceptableSpeedError[BACK] = c->Get<double> (m_configSection,
			"back_acceptableSpeedError", 0);

	maxDeltaDutyCycle = c->Get<double> (m_configSection, "maxDeltaDutyCycle",
			.75);
	m_duty_cycle_delta = c->Get<double> (m_configSection, "m_duty_cycle_delta",
			.25);
	//max_voltage[FRONT] = c->Get<double>(m_configSection, "max_voltage_front", 0);
	//max_voltage[BACK] = c->Get<double>(m_configSection, "max_voltage_back", 0);
	
}

void Shooter::Log()
{

}

void Shooter::CheckError(int roller)
{

	if (fabs(m_PIDs[roller].getError()) > acceptableSpeedError[roller])
	{
		atSpeedCounter[roller] = 0;
		atSpeed[roller] = false;

	}
	else
	{
		atSpeedCounter[roller]++;
		if (atSpeedCounter[roller] > requiredCycles)
		{
			atSpeed[roller] = true;
		}
	}

}

void Shooter::LimitCurrent(int roller)
{
	
	m_PIDs[roller].setInput(m_speed[roller]);
	
	PIDOut[roller] = m_PIDs[roller].update(1.0 / RobotConfig::LOOP_RATE);
	
	double desiredDutyCycle = m_componentData->shooterData->GetDesiredSpeed((Roller)roller) / m_max_speed[roller];
	double maxDutyCycle = (m_speed[roller] / m_max_speed[roller]* RobotConfig::Shooter::MAX_VOLTAGE 
	    + maxDeltaDutyCycle * system_voltage) / system_voltage;
	double appliedDutyCycle = min(desiredDutyCycle, maxDutyCycle);
	m_jaguar[roller]->ConfigMaxOutputVoltage(appliedDutyCycle * RobotConfig::Shooter::MAX_VOLTAGE);
	
	m_jaguar[roller]->SetDutyCycle(PIDOut[roller]);
	
}
