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
	m_jaguars[OUTER] = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_A,
			"ShooterFront");
	m_jaguars[INNER] = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_B,
			"ShooterBack");
	m_encs[OUTER] = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT_A);
	m_encs[OUTER]->Start();
	m_encs[INNER] = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT_B);
	m_encs[INNER]->Start();
	m_pneumatics = Pneumatics::Instance();

	atSpeedCounter[OUTER] = 0;
	atSpeedCounter[INNER] = 0;

	atSpeed[OUTER] = false;
	atSpeed[INNER] = false;
 
	//m_proximity = new DigitalInput(RobotConfig::Digital::PROXIMITY_B);

	requiredCyclesAtSpeed = 0;
	acceptableSpeedError[OUTER] = 0;
	acceptableSpeedError[INNER] = 0;
	lastSpeed = 0;
	m_speeds[OUTER] = 0;
	m_speeds[INNER] = 0;
	
	m_maxNormalizedCurrent = 0.0;
	
	Configure();

}

Shooter::~Shooter()
{
	DELETE(m_jaguars[OUTER]);
	DELETE(m_jaguars[INNER]);
}

void Shooter::onEnable()
{

}

void Shooter::onDisable()
{
	m_jaguars[OUTER]->SetDutyCycle(0.0F);
	m_jaguars[INNER]->SetDutyCycle(0.0F);
}

void Shooter::enabledPeriodic()
{	
	ManageShooterWheel(OUTER);
	ManageShooterWheel(INNER);
	
//	AsyncPrinter::Printf("outer %.2f, inner %.2f\n", m_PIDs[OUTER].getInput(), m_PIDs[INNER].getInput());
	
	double frisbee_detected = 1;//m_proximity->Get() == 0;
	if(atSpeed[OUTER] && atSpeed[INNER])
	{
		if (m_componentData->shooterData->GetShooterSetting() == CONTINOUS)
		{
			bool isExtended = m_pneumatics->GetStorageExitState();
			if (isExtended)
			{
				//if the speed of the first shooter wheel drops a ton. (Defined by a threshold read fromconfig)
				if(m_speeds[INNER] < (lastSpeed - frisbeeDetectionThreshold))
					m_pneumatics->setStorageExit(RETRACTED);
			}
			else
			{
				if (frisbee_detected)
					m_pneumatics->setStorageExit(EXTENDED); //extend
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
	lastSpeed = m_speeds[INNER];
	
	//m_speed_back = Util::Clamp<double>(m_speed_back, 0, m_max_speed * 1.3);

	// TODO: change shooter speed based on orientation
}

void Shooter::ManageShooterWheel(int roller)
{
	m_speeds[roller] = (double) (m_encs[roller]->GetStopped()) ? 0.0 : (60.0 / m_encs[roller]->GetPeriod());
//	static int last = m_encs[roller]->Get();
//	if (m_encs[roller]->Get() == last)
//		AsyncPrinter::Printf("Old Shooter wheel data D:\n");
	
	m_PIDs[roller].setInput(m_speeds[roller]);
	//TODO fixme, add a switch
	//	m_PIDs[roller].setSetpoint(m_componentData->shooterData->GetDesiredSpeed((Roller)roller));
//	m_PIDs[roller].setSetpoint(100000);
	
	m_PIDs[roller].setSetpoint(m_target_speed[roller]);
	
	double out = m_PIDs[roller].update(1.0 / RobotConfig::LOOP_RATE) / m_max_speed[roller] ; //out is a normalized voltage
	
	double maxOut = m_speeds[roller] + m_maxNormalizedCurrent;
	
	//now lets limit the current
	out = Util::Min<double>(out, maxOut);
	
	if (out < 0.0)
		out = 0.0;// Don't do reverse power
	
//	AsyncPrinter::Printf("%d: %.2f\n", out);
	out /= (DriverStation::GetInstance()->GetBatteryVoltage() / RobotConfig::MAX_VOLTAGE);
	
	m_jaguars[roller]->SetDutyCycle(out);
	m_jaguars[roller]->SetVoltageRampRate(0.0);
	
	if(fabs(m_PIDs[roller].getError()) < acceptableSpeedError[roller])
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
	m_jaguars[OUTER]->SetDutyCycle(0.0F);
	m_jaguars[INNER]->SetDutyCycle(0.0F);
}

void Shooter::Configure()
{
	ConfigManager * c = ConfigManager::Instance();
	m_max_speed[OUTER] = c->Get<double> (m_configSection, "outer_maxSpeed",
			7400);
	m_max_speed[INNER] = c->Get<double> (m_configSection, "inner_maxSpeed", 6000);

	
	m_target_speed[OUTER] = c->Get<double> (m_configSection, "outer_speedSetpoint",
			6040);
	m_target_speed[INNER] = c->Get<double> (m_configSection, "inner_speedSetpoint", 4400);

	//TODO: Change default values.
	requiredCyclesAtSpeed = c->Get<int> (m_configSection, "requiredCycles", 2);
	acceptableSpeedError[OUTER] = c->Get<double> (m_configSection,
			"front_acceptableSpeedError", 100);
	acceptableSpeedError[INNER] = c->Get<double> (m_configSection,
			"back_acceptableSpeedError", 100);
	
	frisbeeDetectionThreshold = c->Get<double> (m_configSection, "shooterSpeedDrop", 0);
	
	m_maxNormalizedCurrent = c->Get<double>(m_configSection, "normalizedCurrentThreshold", 1.0);
	
	ConfigurePIDObject(&m_PIDs[INNER], "InnerWheelPID", 1.0);
	ConfigurePIDObject(&m_PIDs[OUTER], "OuterWheelPID", 1.0);
}

void Shooter::Log()
{

}

void Shooter::ConfigurePIDObject(PID *pid, std::string objName, bool feedForward)
{
	double p = m_config->Get<double>(Component::GetName(), objName + "_P", 0.002);
	double i = m_config->Get<double>(Component::GetName(), objName + "_I", 0.0);
	double d = m_config->Get<double>(Component::GetName(), objName + "_D", 0.0);
	
	pid->setParameters(p, i, d, 1.0, 0.87, feedForward);//super high decay, this makes it just like a filter. If you want it to act more like an integral you reduce the decay. This must be tuned. 
}
