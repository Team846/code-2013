#include "Shooter.h"
#include "../ComponentData/ShooterData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../ComponentData/ShooterData.h"
#include "../Components/Climber.h"
#include "../Log/LogManager.h"

using namespace data;
using namespace data::shooter;

#define RELIABLE_SHOOTING 0
#define FILTERED_SENSOR 0
#define TWOSPEED 1
#define SENSOR_DENOISE_RATE 400.0

//Front of the pyramid is 3400, 4040

Shooter::Shooter() :
	Component("Shooter", DriverStationConfig::DigitalIns::SHOOTER, true),
			m_configSection("Shooter"),
			m_inner_file("/ShooterInner.log"),
			m_outer_file("/ShooterOuter.log"),
			m_sensorProcessingNotifier((TimerEventHandler) Shooter::DeNoiseSensorEntry, this)
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
 
	m_proximity = new DigitalInput(RobotConfig::Digital::PROXIMITY_SHOOTER );

	requiredCyclesAtSpeed = 0;
	acceptableSpeedError[OUTER] = 0;
	acceptableSpeedError[INNER] = 0;
	lastSpeed = 0;
	m_speedsRPM[OUTER] = 0;
	m_speedsRPM[INNER] = 0;
	
	m_maxNormalizedCurrent = 0.0;
	
	m_fireState = FIRING_OFF;
	m_cyclesToContinueRetracting = 0;
	m_sensorDeNoisingCycle = 0;
	
	m_isSensorTrue = false;
	
	m_ticks = 0;
	
	Configure();
	
	m_sensorProcessingNotifier.StartPeriodic(1.0 / SENSOR_DENOISE_RATE);
} 

Shooter::~Shooter()
{
	DELETE(m_jaguars[OUTER]);
	DELETE(m_jaguars[INNER]);
}

void Shooter::onEnable()
{
	m_outer_file.open("/ShooterOuter.log");
	m_inner_file.open("/ShooterInner.log");
}

void Shooter::onDisable()
{
//	m_outer_file.close();
//	m_inner_file.close();
	m_jaguars[OUTER]->SetDutyCycle(0.0F);
	m_jaguars[INNER]->SetDutyCycle(0.0F);
}

//inner_speedSetpoint=4400
//outer_speedSetpoint=6040

void Shooter::enabledPeriodic()
{	
	if (m_componentData->shooterData->IsEnabled())
	{
		if (m_componentData->shooterData->ShouldLauncherBeHigh())
		{
			m_pneumatics->setShooterAngler(EXTENDED);
	//		AsyncPrinter::Printf("High\n");
		}
		else
		{
	//		AsyncPrinter::Printf("Low\n");
			m_pneumatics->setShooterAngler(RETRACTED);
		}
			
		
	//	AsyncPrinter::Printf("Period %.5f\n", m_encs[INNER]->GetPeriod());
		ManageShooterWheel(OUTER);
		ManageShooterWheel(INNER);
		
		static int e = 0;
		e++;
	//	AsyncPrinter::Printf("%d: inner Speed %.2f, inner error %.2f, inner out %.2f\n", ++e, m_PIDs[INNER].getInput(), m_PIDs[INNER].getError(), m_PIDs[INNER].getOutput()/ m_max_speed[INNER] );
	//	AsyncPrinter::Printf("%d: inner Speed %.2f, inner error %.2f, inner out %.2f\n", ++e, m_PIDs[OUTER].getInput(), m_PIDs[OUTER].getError(), m_PIDs[OUTER].getOutput()/ m_max_speed[OUTER] );
		
		switch (m_componentData->shooterData->GetShooterSetting())
		{
		case CONTINOUS:
				m_pneumatics->setStorageExit(EXTENDED);
				switch(m_fireState)
				{
				case FIRING_OFF:
					if (atSpeed[OUTER] && atSpeed[INNER])
					{
						m_fireState = RETRACT_LOADER_WAIT_FOR_LIFT;
						m_cyclesToContinueRetracting = requiredCyclesDown ;
						m_pneumatics->setStorageExit(RETRACTED);
						startShotTime = e;
						
						m_timer = 50 * 1.3;
					}
					else if (e % 20 == 0)
						AsyncPrinter::Printf("Not at speed %.0f, %.0f\n", m_speedsRPM[INNER], m_speedsRPM[OUTER]);
					break;
				case RETRACT_LOADER_WAIT_FOR_LIFT:
					m_timer--;
	//				if (m_cyclesToContinueRetracting > 0)
#if RELIABLE_SHOOTING
					if (!m_proximity->Get() && m_timer > 0)//keep waiting
#else 
	#if FILTERED_SENSOR
					if (!m_isSensorTrue)//keep waiting
	#else
					if (!m_proximity->Get())//keep waiting
	#endif
#endif
					{
						m_pneumatics->setStorageExit(RETRACTED);
						m_cyclesToContinueRetracting--;
					}
					else
					{
						m_pneumatics->setStorageExit(RETRACTED);
	//					m_pneumatics->setStorageExit(EXTENDED);
						m_fireState = RETRACT_LOADER_WAIT_FOR_FALL;
					}
					break;
				case RETRACT_LOADER_WAIT_FOR_FALL:
#if RELIABLE_SHOOTING
					if (m_proximity->Get() && m_timer > 0)//keep waiting
#else
	#if FILTERED_SENSOR
					if (m_isSensorTrue)//keep waiting
	#else
					if (m_proximity->Get())//keep waiting
	#endif
#endif
					{
						m_pneumatics->setStorageExit(RETRACTED);
						m_cyclesToContinueRetracting--;
					}
					else
					{
						m_pneumatics->setStorageExit(EXTENDED);
						m_fireState = EXTEND_LOADER;
						m_timer = 50;
					}
					break;
				case EXTEND_LOADER:
						m_pneumatics->setStorageExit(EXTENDED);
						m_timer--;
#if RELIABLE_SHOOTING
						if (m_PIDs[INNER].getError() > frisbeeDetectionThreshold || m_timer < 0)
#else
						if (m_PIDs[INNER].getError() > frisbeeDetectionThreshold)
#endif
						{
							AsyncPrinter::Printf("Fired with newSpeed = %.0f, lastSpeed = %.0f taking %d cycles\n", m_speedsRPM[INNER], lastSpeed, e - startShotTime);
							m_fireState = RETRACT_LOADER_WAIT_FOR_LIFT;
							m_cyclesToContinueRetracting = requiredCyclesDown ;
							m_pneumatics->setStorageExit(RETRACTED);
							
							if (m_timer >= 0)
								m_componentData->shooterData->DecrementFrisbeeCounter();
							
							startShotTime = e;
						}
	//					else
	//						AsyncPrinter::Printf("Speed drop %.3f\n", lastSpeed - m_speeds[INNER]);
					break;
				}
	//			AsyncPrinter::Printf("Out\n");
			break;
		case ONCE:
			m_pneumatics->setStorageExit(RETRACTED);
			break;
		case OFF:
//				AsyncPrinter::Printf("off\n");
	//			AsyncPrinter::Printf("IN\n");
				m_pneumatics->setStorageExit(EXTENDED);
				m_fireState = FIRING_OFF;
			break;
		}
		lastSpeed = m_speedsRPM[INNER];
		AsyncPrinter::Printf("Speed %.3f\n", m_speedsRPM[OUTER]);
		
	//	double frisbee_detected = 1;//m_proximity->Get() == 0;
	//	if(atSpeed[OUTER] && atSpeed[INNER])
	//	{
	//		if (m_componentData->shooterData->GetShooterSetting() == CONTINOUS)
	//		{
	//			bool isExtended = m_pneumatics->GetStorageExitState();
	//			if (isExtended)
	//			{
	//				//if the speed of the first shooter wheel drops a ton. (Defined by a threshold read fromconfig)
	//				if(m_speeds[INNER] < (lastSpeed - frisbeeDetectionThreshold))
	//					m_pneumatics->setStorageExit(RETRACTED);
	//			}
	//			else
	//			{
	//				if (frisbee_detected)
	//					m_pneumatics->setStorageExit(EXTENDED); //extend
	//			}
	//		} else if(m_componentData->shooterData->GetShooterSetting() == ONCE)
	//		{
	//			m_pneumatics->setStorageExit(RETRACTED);
	//			bool m_isExtended = false;
	//
	//			if(!m_isExtended)
	//			{
	//				if(frisbee_detected)
	//				{
	//					m_pneumatics->setStorageExit(EXTENDED);
	//					m_isExtended = true;
	//				}
	//			} 
	//			else
	//			{
	//				m_pneumatics->setStorageExit(RETRACTED);
	//				m_componentData->shooterData->SetShooterSetting(OFF);
	//			}
	//		} 
	//		else
	//		{
	//			m_pneumatics->setStorageExit(RETRACTED);
	//		}
	//	}
	//	else
	//	{
	//		AsyncPrinter::Printf("Not at speed\n");
	//		m_pneumatics->setStorageExit(RETRACTED);
	//	}
		
		//m_speed_back = Util::Clamp<double>(m_speed_back, 0, m_max_speed * 1.3);
	
		// TODO: change shooter speed based on orientation
	}
	else
	{
		disabledPeriodic();
	}
}
#define PATCH_BAD_SPEED_DATA

void Shooter::ManageShooterWheel(int roller)
{
	//TODO assert to avoid out of bounds 
	double tempSpeedRPM = (double) (m_encs[roller]->GetStopped()) ? 0.0 : (60.0 / m_encs[roller]->GetPeriod());

#ifdef PATCH_BAD_SPEED_DATA
	if (tempSpeedRPM > m_max_speed[roller])
		tempSpeedRPM = m_speedsRPM[roller];
#endif
	
	m_speedsRPM[roller] = tempSpeedRPM;
//	static int last = m_encs[roller]->Get();
//	if (m_encs[roller]->Get() == last)
//		AsyncPrinter::Printf("Old Shooter wheel data D:\n");
	
	m_PIDs[roller].setInput(m_speedsRPM[roller]);
	//TODO fixme, add a switch 	
	//	m_PIDs[roller].setSetpoint(m_componentData->shooterData->GetDesiredSpeed((Roller)roller));
//	m_PIDs[roller].setSetpoint(100000);
	
#if TWOSPEED
	if (m_componentData->shooterData->ShouldLauncherBeHigh()) //low speed. meantime
	{
		m_PIDs[roller].setSetpoint(m_speed_setpoints[roller][HIGH]);
//		AsyncPrinter::Printf("Setpoint: %.0f\n", m_PIDs[roller].getSetpoint());
	}
	else
	{
		m_PIDs[roller].setSetpoint(m_speed_setpoints[roller][LOW]);
	}
#else
	if (!m_componentData->shooterData->ShouldLauncherBeHigh())
	{
		if (roller == OUTER)
		{
			m_PIDs[roller].setSetpoint(5300);
		}
		else
		{
			m_PIDs[roller].setSetpoint(4900);
		}
	}
	else
	{
		m_PIDs[roller].setSetpoint(m_target_speed[roller]);
	}
#endif
	
	double out = m_PIDs[roller].update(1.0 / RobotConfig::LOOP_RATE) / m_max_speed[roller] ; //out is a normalized voltage
	
	double maxOut = m_speedsRPM[roller] + m_maxNormalizedCurrent;
	
	//now lets limit the current
	out = Util::Min<double>(out, maxOut);
	
	if (out < 0.0)
		out = 0.0;// Don't do reverse power
	
//	AsyncPrinter::Printf("%d: %.2f\n", out);
	out /= (DriverStation::GetInstance()->GetBatteryVoltage() / RobotConfig::MAX_VOLTAGE);
	
	if (m_PIDs[roller].getError() < -400)
		m_jaguars[roller]->ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	else 
		m_jaguars[roller]->ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
		
	m_jaguars[roller]->SetDutyCycle(out);
	m_jaguars[roller]->SetVoltageRampRate(0.0);
	
	static int e = 0;
//	if (++e % 5 == 0)
//		AsyncPrinter::Printf("Error %.0f\n", m_PIDs[roller].getError());
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
	m_ticks++;
	m_outer_file << m_ticks << "," << m_speedsRPM[OUTER] << "," << m_jaguars[OUTER]->GetOutputCurrent() << "\n";
	m_inner_file << m_ticks << "," << m_speedsRPM[INNER] << "," << m_jaguars[INNER]->GetOutputCurrent() << "\n";
	
}

void Shooter::disabledPeriodic()
{
	m_outer_file.close();
	m_inner_file.close();
	if (m_componentData->shooterData->ShouldLauncherBeHigh())
	{
		m_pneumatics->setShooterAngler(EXTENDED);
//		AsyncPrinter::Printf("High\n");
	}
	else
	{
//		AsyncPrinter::Printf("Low\n");
		m_pneumatics->setShooterAngler(RETRACTED);
	}
	
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
	
	
	m_speed_setpoints[OUTER][LOW] = c->Get<double>(m_configSection, "outer_highSpeedSetpoint", 5300);
	m_speed_setpoints[INNER][LOW] = c->Get<double>(m_configSection, "inner_highSpeedSetpoint", 4900);

	m_speed_setpoints[OUTER][HIGH] = c->Get<double>(m_configSection, "outer_lowSpeedSetpoint", 3100);
	m_speed_setpoints[INNER][HIGH] = c->Get<double>(m_configSection, "inner_lowSpeedSetpoint", 2800);

	//TODO: Change default values.
	requiredCyclesAtSpeed = c->Get<int> (m_configSection, "requiredCycles", 2);
	acceptableSpeedError[OUTER] = c->Get<double> (m_configSection,
			"outer_acceptableSpeedError", 30);
	acceptableSpeedError[INNER] = c->Get<double> (m_configSection,
			"inner_acceptableSpeedError", 30);
	
	frisbeeDetectionThreshold = c->Get<double> (m_configSection, "shooterSpeedDrop", 100);
	
	m_maxNormalizedCurrent = c->Get<double>(m_configSection, "normalizedCurrentThreshold", 1.0);
	
	
	requiredCyclesDown = c->Get<double>(m_configSection, "cyclesToRetractLoader", 50);
	
	ConfigurePIDObject(&m_PIDs[INNER], "InnerWheelPID", 1.0);
	ConfigurePIDObject(&m_PIDs[OUTER], "OuterWheelPID", 1.0);
}

void Shooter::ConfigurePIDObject(PID *pid, std::string objName, bool feedForward)
{
	double p = m_config->Get<double>(Component::GetName(), objName + "_P", 0.002);
	double i = m_config->Get<double>(Component::GetName(), objName + "_I", 0.0);
	double d = m_config->Get<double>(Component::GetName(), objName + "_D", 0.0);
	
	pid->setParameters(p, i, d, 1.0, 0.87, feedForward);//super high decay, this makes it just like a filter. If you want it to act more like an integral you reduce the decay. This must be tuned. 
}

void Shooter::DeNoiseSensorEntry(void * param)
{
	Shooter *shooter = (Shooter*) param;
	shooter->DeNoiseSensor();
}

void Shooter::DeNoiseSensor()
{
	m_sensorVals[m_sensorDeNoisingCycle] = m_proximity->Get();
	int numTrue = 0, numFalse = 0;
	
	for (int i = 0; i < NUM_SENSOR_VALS; i++)
	{
		if (m_sensorVals[i])
			numTrue++;
		else
			numFalse++;
	}
	
	m_sensorDeNoisingCycle++;
	if (m_sensorDeNoisingCycle >= NUM_SENSOR_VALS)
	{
		m_sensorDeNoisingCycle = 0;
	}
	
	if (numTrue > numFalse)
		m_isSensorTrue = true;
	else
		m_isSensorTrue = false;
}

void Shooter::Log()
{
}
