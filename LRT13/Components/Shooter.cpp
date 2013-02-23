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

	requiredCyclesAtSpeed = 0;
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
	ManageShooterWheel(FRONT);
	ManageShooterWheel(BACK);
	
	frisbee_detected = m_proximity->Get() == 0;
	if (m_componentData->shooterData->ShouldBeShootingContinuous())
	{
		static bool m_isExtended;
		if (m_isExtended)
		{
			//if the speed of the first shooter wheel drops a ton. (Defined by a threshold read fromconfig)
			//retract
			m_isExtended = false;
		}
		else
		{
			if (frisbee_detected)
			{
				//extend
				m_isExtended = true;
			}
		}
	}
	
	//m_speed_back = Util::Clamp<double>(m_speed_back, 0, m_max_speed * 1.3);

	// TODO: change shooter speed based on orientation

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
	
	
}

void Shooter::ManageShooterWheel(int roller)
{
	double currSpeed = (double) (m_encs[roller]->GetStopped()) ? 0.0 : (60.0 / 2.0 / m_encs[roller]->GetPeriod());
	m_PIDs[roller].setInput(currSpeed/m_max_speed[roller]);
	m_PIDs[roller].setSetpoint(m_componentData->shooterData->GetDesiredSpeed(roller));
	
	double out = m_PIDs[roller].update(1.0 / RobotConfig::LOOP_RATE); //out is a normalized voltage
	
	double maxOut = currSpeed + m_maxNormalizedCurrent;
	
	//now lets limit the current
	out = Util::Min<double>(out, maxOut);
	out /= (DriverStation::GetInstance()->GetBatteryVoltage() / RobotConfig::MAX_VOLTAGE);
	
	m_jaguars[roller]->SetDutyCycle(out);
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
	requiredCyclesAtSpeed = c->Get<int> (m_configSection, "requiredCycles", 9);
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
		if (atSpeedCounter[roller] > requiredCyclesAtSpeed)
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
