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
//#define OPEN_LOOP
#define TALON

//Front of the pyramid is 3400, 4040

Shooter::Shooter() :
			Component("Shooter", DriverStationConfig::DigitalIns::SHOOTER, true),
			m_configSection("Shooter"),
			m_inner_file("/ShooterInner.csv"),
			m_outer_file("/ShooterOuter.csv"),
			m_sensorProcessingNotifier(
					(TimerEventHandler) Shooter::DeNoiseSensorEntry, this)
{
#ifndef TALON
	m_jaguars[OUTER] = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_B,
			"ShooterFront");
	m_jaguars[INNER] = new AsyncCANJaguar(RobotConfig::CAN::SHOOTER_A,
			"ShooterBack");
#else
	m_talons[OUTER] = new LRTTalon(RobotConfig::PWM::SHOOTER_B, "ShooterFront", 9);
	m_talons[INNER] = new LRTTalon(RobotConfig::PWM::SHOOTER_A, "ShooterBack", 10);
#endif

#ifndef TALON
	m_jaguars[OUTER]->setCollectionFlags(AsyncCANJaguar::OUTCURR);
	m_jaguars[INNER]->setCollectionFlags(AsyncCANJaguar::OUTCURR);
#endif

	m_outerSensor = new DigitalInput((UINT32) RobotConfig::Digital::HALL_EFFECT_B);
	m_encs[OUTER] = new Counter(m_outerSensor);
	m_encs[OUTER]->Start();
	m_encs[OUTER]->SetMaxPeriod(60 / 100.0); // Period of 100 RPM; minimum speed we can read -RC 4/12/13
	m_encs[INNER] = new Counter((UINT32) RobotConfig::Digital::HALL_EFFECT_A);
	m_encs[INNER]->Start();
	m_encs[INNER]->SetMaxPeriod(60 / 100.0); // Period of 100 RPM; minimum speed we can read -RC 4/12/13
	m_angler = new Pneumatics(RobotConfig::Solenoid::SHOOTER_ANGLER_A, RobotConfig::Solenoid::SHOOTER_ANGLER_B, "ShooterAngler");
	m_pusher = new Pneumatics(RobotConfig::Solenoid::STORAGE_EXIT_A, RobotConfig::Solenoid::STORAGE_EXIT_B, 2, "StorageExit");
	
	atSpeedCounter[OUTER] = 0;
	atSpeedCounter[INNER] = 0;

	atSpeed[OUTER] = false;
	atSpeed[INNER] = false;

	m_proximity = new DigitalInput(RobotConfig::Digital::PROXIMITY_SHOOTER);

	requiredCyclesAtSpeed = 0;
	acceptableSpeedErrorNormalized[OUTER] = 0;
	acceptableSpeedErrorNormalized[INNER] = 0;
	m_speedsRPM[OUTER] = 0;
	m_speedsRPM[INNER] = 0;
	m_speed_setpoints[OUTER][LOW] = 0;
	m_speed_setpoints[OUTER][HIGH] = 0;
	m_speed_setpoints[INNER][LOW] = 0;
	m_speed_setpoints[INNER][HIGH] = 0;
	m_periods[OUTER] = 0;
	m_errorIntegrals[INNER] = 0;
	m_errorIntegrals[OUTER] = 0;
	m_periods[INNER] = 0;

	m_maxNormalizedCurrent = 0.0;

	m_sensorDeNoisingCycle = 0;
	m_isSensorTrue = false;

	m_fireState = FIRING_OFF;
	m_cyclesToContinueRetracting = 0;
	// begin denoise removal
	//	m_sensorDeNoisingCycle = 0;
	//	m_isSensorTrue = false
	// end denoise removal

	m_ticks = 0;

	m_flashlight = new DigitalOutput(RobotConfig::Digital::FLASHLIGHT); // Flashlight change
	m_flashlightPWM = new PWM(RobotConfig::PWM::FLASHLIGHT);
	m_flashlightBrightness = 0.5;

	Configure();

	m_sensorProcessingNotifier.StartPeriodic(1.0 / SENSOR_DENOISE_RATE);
	firingWaitTicks = 0;
	m_sensorStableTime = 0;
	m_sensorOK = false;
	frisbeeExited = false;
	frisbeeExitedLastCycle = false;
	table = NetworkTable::GetTable("RobotData");
	m_timeoutCounter = 0;
}

Shooter::~Shooter()
{
#ifndef TALON
	DELETE(m_jaguars[OUTER]);
	DELETE(m_jaguars[INNER]);
#endif
}

void Shooter::onEnable()
{
	if (!m_outer_file.is_open())
		m_outer_file.open("/ShooterOuter.csv");
	if (!m_inner_file.is_open())
		m_inner_file.open("/ShooterInner.csv");
}

void Shooter::onDisable()
{
	m_outer_file.flush();
	m_inner_file.flush();
	if (m_outer_file.is_open())
		m_outer_file.close();
	if (m_inner_file.is_open())
		m_inner_file.close();
	m_ticks = 0;
#ifndef TALON
	m_jaguars[OUTER]->SetDutyCycle(0.0F);
	m_jaguars[INNER]->SetDutyCycle(0.0F);
#endif
}

//inner_speedSetpoint=4400
//outer_speedSetpoint=6040

void Shooter::enabledPeriodic()
{
	
	if(m_componentData->shooterData->IsFlashlightEnabled())
	{
		m_flashlight->Set(1); 
		m_flashlightPWM->SetRaw((int)m_flashlightBrightness * 255);
	}
	else
	{
		m_flashlight->Set(0); 
		m_flashlightPWM->SetRaw((int)128);

	}
	
	if (m_componentData->shooterData->IsEnabled())
	{
		
		if (m_componentData->shooterData->ShouldLauncherBeHigh())
		{
			m_angler->Set(EXTENDED, true);
			//		AsyncPrinter::Printf("High\n");
		}
		else
		{
			//		AsyncPrinter::Printf("Low\n");
			m_angler->Set(RETRACTED, true);
		}

		//	AsyncPrinter::Printf("Period %.5f\n", m_encs[INNER]->GetPeriod());
		Profiler::BeginActivity("ManageShooterWheel(OUTER)");
		ManageShooterWheel(OUTER);
		Profiler::End("ManageShooterWheel(OUTER)");
		Profiler::BeginActivity("ManageShooterWheel(INNER)");
		ManageShooterWheel(INNER);
		Profiler::End("ManageShooterWheel(INNER)");

		static int e = 0;
		e++;

		static bool lastVal = m_proximity->Get();
		if (lastVal != m_proximity->Get())
			AsyncPrinter::Printf("shooter loading sensor %d\n",
					m_proximity->Get());
		lastVal = m_proximity->Get();
		//	AsyncPrinter::Printf("%d: inner Speed %.2f, inner error %.2f, inner out %.2f\n", ++e, m_PIDs[INNER].getInput(), m_PIDs[INNER].getError(), m_PIDs[INNER].getOutput()/ m_max_speed[INNER] );
		//	AsyncPrinter::Printf("%d: inner Speed %.2f, inner error %.2f, inner out %.2f\n", ++e, m_PIDs[OUTER].getInput(), m_PIDs[OUTER].getError(), m_PIDs[OUTER].getOutput()/ m_max_speed[OUTER] );
		static bool lastFiring = false;
		switch (m_componentData->shooterData->GetShooterSetting())
		{
		case CONTINUOUS:
			//				m_pneumatics->setStorageExit(EXTENDED);
			//				AsyncPrinter::Printf("FireState: %d\n", m_fireState);
			//				switch(m_fireState)
			//				{
			//				case FIRING_OFF:
			//					if (atSpeed[OUTER] && atSpeed[INNER])
			//					{
			//						m_fireState = RETRACT_LOADER_WAIT_FOR_LIFT;
			//						m_cyclesToContinueRetracting = requiredCyclesDown ;
			//						m_pneumatics->setStorageExit(RETRACTED);
			//						startShotTime = e;
			//						
			//						m_timer = 50 * 1.3;
			//					}
			//					else if (e % 20 == 0)
			//						AsyncPrinter::Printf("Not at speed %.0f, %.0f\n", m_speedsRPM[INNER], m_speedsRPM[OUTER]);
			//					break;
			//				case RETRACT_LOADER_WAIT_FOR_LIFT:
			//					m_timer--;
			//	//				if (m_cyclesToContinueRetracting > 0)
			//#if RELIABLE_SHOOTING
			//					if (!m_proximity->Get() && m_timer > 0)//keep waiting
			//#else 
			//	#if FILTERED_SENSOR
			//					if (!m_isSensorTrue)//keep waiting
			//	#else
			//					if (!m_proximity->Get())//keep waiting
			//	#endif
			//#endif
			//					{
			//						m_pneumatics->setStorageExit(RETRACTED);
			//						m_cyclesToContinueRetracting--;
			//					}
			//					else
			//					{
			//						m_pneumatics->setStorageExit(RETRACTED);
			//	//					m_pneumatics->setStorageExit(EXTENDED);
			//						m_fireState = RETRACT_LOADER_WAIT_FOR_FALL;
			//					}
			//					break;
			//				case RETRACT_LOADER_WAIT_FOR_FALL:
			//#if RELIABLE_SHOOTING
			//					if (m_proximity->Get() && m_timer > 0)//keep waiting
			//#else
			//	#if FILTERED_SENSOR
			//					if (m_isSensorTrue)//keep waiting
			//	#else
			//					if (m_proximity->Get())//keep waiting
			//	#endif
			//#endif
			//					{
			//						m_pneumatics->setStorageExit(RETRACTED);
			//						m_cyclesToContinueRetracting--;
			//					}
			//					else/* if(atSpeed[INNER] && atSpeed[OUTER])*/
			//					{
			//					
			//						m_pneumatics->setStorageExit(EXTENDED);
			//						m_fireState = EXTEND_LOADER;
			//						m_timer = 50;
			//					}
			//					break;
			//				case EXTEND_LOADER:
			//						m_pneumatics->setStorageExit(EXTENDED);
			//						m_timer--;
			//						AsyncPrinter::Printf("normalized error: %f, threshold: %f\n", m_errorsNormalized[INNER], frisbeeDetectionThreshold);
			//#if RELIABLE_SHOOTING
			//						if (m_errors[INNER] > frisbeeDetectionThreshold || m_timer < 0)
			//#else
			//						if (fabs(m_errorsNormalized[INNER]) > frisbeeDetectionThreshold || m_timer < 0)
			//#endif
			//							
			//						{
			//							AsyncPrinter::Printf("Outer wheel speed when shooting: %f\n", m_speedsRPM[OUTER]);
			//							AsyncPrinter::Printf("Inner wheel speed when shooting: %f\n", m_speedsRPM[INNER]);
			////							AsyncPrinter::Printf("Fired with newSpeed = %.0f, lastSpeed = %.0f taking %d cycles\n", m_speedsRPM[INNER], lastSpeed, e - startShotTime);
			//							m_fireState = RETRACT_LOADER_WAIT_FOR_LIFT;
			//							m_cyclesToContinueRetracting = requiredCyclesDown ;
			////							m_pneumatics->setStorageExit(RETRACTED);
			////							
			//							if (m_timer >= 0)
			//								m_componentData->shooterData->DecrementFrisbeeCounter();
			//							
			//							startShotTime = e;
			//						}
			//	//					else
			//	//						AsyncPrinter::Printf("Speed drop %.3f\n", lastSpeed - m_speeds[INNER]);
			//					break;
			//				}
			//	//			AsyncPrinter::Printf("Out\n");

			//				switch(m_fireState)
			//				{
			//				case FIRING_OFF:
			//					firingWaitTicks=0;
			//					m_pneumatics->setStorageExit(RETRACTED); // move pusher back
			//					m_fireState=RETRACT_LOADER_WAIT_FOR_LIFT; 
			//					break;
			//					
			//				case RETRACT_LOADER_WAIT_FOR_LIFT:
			//					firingWaitTicks++;
			//					if(m_proximity->Get()){ // frisbee lifted
			//					m_fireState=	RETRACT_LOADER_WAIT_FOR_FALL;
			//					
			//					}
			//					break;
			//				case RETRACT_LOADER_WAIT_FOR_FALL:
			//					if(!m_proximity->Get()){ // frisbee fallen
			//						m_fireState=EXTEND_LOADER;
			//						firingWaitTicks=0;
			//					}
			//					break;
			//					
			//				case EXTEND_LOADER:
			//					m_pneumatics->setStorageExit(EXTENDED); // fire frisbee
			//					firingWaitTicks++;
			//					if(firingWaitTicks==10){
			//						m_fireState=FIRING_OFF;
			//					
			//					}
			//					break;
			//				}

			switch (m_fireState)
			{
			case FIRING_OFF:
				firingWaitTicks = 0;
				if (atSpeed[OUTER] && atSpeed[INNER])
				{
					m_pusher->Set(RETRACTED);
					m_fireState = RETRACT_LOADER_WAIT_FOR_LIFT;
				}
				break;
			case RETRACT_LOADER_WAIT_FOR_LIFT:
				m_pusher->Set(RETRACTED);
				firingWaitTicks++;
				if (m_proximity->Get() && m_sensorOK)
				{
					m_fireState = RETRACT_LOADER_WAIT_FOR_FALL;
				}
				if (firingWaitTicks >= retractWait && atSpeed[OUTER]
						&& atSpeed[INNER])
				{
					m_pusher->Set(EXTENDED);
					m_fireState = EXTEND_LOADER;
					firingWaitTicks = 0;
				}
				break;
			case RETRACT_LOADER_WAIT_FOR_FALL:
				firingWaitTicks++;
				if ((firingWaitTicks >= retractWait || (!m_proximity->Get()
						&& m_sensorOK)) && atSpeed[OUTER] && atSpeed[INNER])
				{
					m_pusher->Set(EXTENDED);
					m_fireState = EXTEND_LOADER;
					firingWaitTicks = 0;
				}
				break;
			case EXTEND_LOADER:
				m_pusher->Set(EXTENDED);
				firingWaitTicks++;
				if (firingWaitTicks >= extendWait)
				{
					m_pusher->Set(RETRACTED);
					m_fireState = FIRING_OFF;
				}
				break;
			}
			break;
		case ONCE:
			switch (m_fireState)
			{
			case FIRING_OFF:
				firingWaitTicks = 0;
				if (atSpeed[OUTER] && atSpeed[INNER])
				{
					m_pusher->Set(RETRACTED);
					m_fireState = RETRACT_LOADER_WAIT_FOR_LIFT;
				}
				break;
			case RETRACT_LOADER_WAIT_FOR_LIFT:
				m_pusher->Set(RETRACTED);
				firingWaitTicks++;
				if (m_proximity->Get() && m_sensorOK)
				{
					m_fireState = RETRACT_LOADER_WAIT_FOR_FALL;
				}
				if (firingWaitTicks >= retractWait)
				{
					m_pusher->Set(EXTENDED);
					m_fireState = EXTEND_LOADER;
					firingWaitTicks = 0;
				}
				break;
			case RETRACT_LOADER_WAIT_FOR_FALL:
				firingWaitTicks++;
				if ((firingWaitTicks >= retractWait || (!m_proximity->Get()
						&& m_sensorOK)) && atSpeed[OUTER] && atSpeed[INNER])
				{
					m_pusher->Set(EXTENDED);
					m_fireState = EXTEND_LOADER;
				}
				break;
			case EXTEND_LOADER:
				m_pusher->Set(EXTENDED);
				break;
			}
			break;
		case MANUAL:
			if (atSpeed[OUTER] && atSpeed[INNER])
			{
				lastFiring = true;
				m_pusher->Set(RETRACTED);
			}
			break;
		case OFF:
			if (lastFiring)
			{
				lastFiring = false;
				AsyncPrinter::Printf(
						"\t---Outer wheel speed when shooting: %f\n",
						m_speedsRPM[OUTER]);
				AsyncPrinter::Printf(
						"\t---Inner wheel speed when shooting: %f\n",
						m_speedsRPM[INNER]);
				AsyncPrinter::Printf("Firing Wait Ticks: %d\n", firingWaitTicks);
			}
			static bool lastSensor = m_proximity->Get();
			if (lastSensor != m_proximity->Get())
			{
				m_sensorStableTime = 0;
				m_sensorOK = false;
			}
			else
			{
				m_sensorStableTime++;
			}
			if (m_sensorStableTime > 4)
			{
				m_sensorOK = true;
			}
			lastSensor = m_proximity->Get();
			//				AsyncPrinter::Printf("off\n");
			//			AsyncPrinter::Printf("IN\n");
			m_pusher->Set(EXTENDED);
			m_fireState = FIRING_OFF;
			break;
		}
		
		frisbeeExited = !atSpeed[OUTER]; // Speed drop
		if (frisbeeExited && frisbeeExited != frisbeeExitedLastCycle)
		{
			m_componentData->shooterData->IncrementFrisbeeCounter();
		}
		frisbeeExitedLastCycle = frisbeeExited;
		
		//		AsyncPrinter::Printf("Speed %.3f\n", m_speedsRPM[OUTER]);

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
		double timenow = Timer::GetFPGATimestamp();

		SmarterDashboard::Instance()->EnqueueShooterMessage(
				MessageType::FRONT_SHOOTER_DATA_SPEED, timenow,
				m_speedsRPM[OUTER] / m_max_speed[OUTER]);
		SmarterDashboard::Instance()->EnqueueShooterMessage(
				MessageType::BACK_SHOOTER_DATA_SPEED, timenow,
				m_speedsRPM[INNER] / m_max_speed[INNER]);
#ifndef TALON
		SmarterDashboard::Instance()->EnqueueShooterMessage(MessageType::FRONT_SHOOTER_DATA_CURRENT, timenow, m_jaguars[OUTER]->GetOutputCurrent());
		SmarterDashboard::Instance()->EnqueueShooterMessage(
				MessageType::BACK_SHOOTER_DATA_CURRENT, timenow,
				m_jaguars[INNER]->GetOutputCurrent());
#endif

		m_ticks++;
		m_outer_file << (double) (m_ticks / 50.0) << "," << m_speedsRPM[OUTER]
				<< "," << m_periods[OUTER] << "\n";
		m_inner_file << (double) (m_ticks / 50.0) << "," << m_speedsRPM[INNER]
				<< "," << m_periods[INNER] << "\n";
	}
	else
	{
		LCD::Instance()->Print(5, 19, false, "%c", ' ');
		LCD::Instance()->Print(5, 20, false, "%c", ' ');
		fubarDoDisabledPeriodic();
		//		disabledPeriodic();
	}
	table->PutNumber("ShooterInner", m_speedsRPM[INNER] / m_max_speed[INNER]);
	table->PutNumber("ShooterOuter", m_speedsRPM[OUTER] / m_max_speed[OUTER]);
	LCD::Instance()->Print(5, 0, false, "Shooter: %.2f%%",
			100 * m_componentData->shooterData->GetSpeedOffset());
	
}
#define PATCH_BAD_SPEED_DATA

void Shooter::ManageShooterWheel(int roller)
{
	//const double loopPeriod = 1.0 / RobotConfig::LOOP_RATE;
	const double measuredPeriod = m_encs[roller]->GetPeriod();

	if (measuredPeriod == 0.0)
	{
		AsyncPrinter::Printf(
				"[ERROR] Shooter::ManageShooterWheel(): period=0\n");
		return;
	}

	double currentSpeedRPM = m_encs[roller]->GetStopped() ? (0.0) : (60.0
			/ measuredPeriod);
	//double currentSpeedNormalized = currentSpeedRPM / m_max_speed[roller];

	//TODO fixme, add a switch 	
	//	m_PIDs[roller].setSetpoint(m_componentData->shooterData->GetDesiredSpeed((Roller)roller));
	//	m_PIDs[roller].setSetpoint(100000);

#if TWOSPEED
	double speedSetpoint = m_speed_setpoints[roller][LOW]
			* m_componentData->shooterData->GetSpeedOffset();

	if (m_componentData->shooterData->ShouldLauncherBeHigh()) //low speed. meantime
		speedSetpoint = m_speed_setpoints[roller][HIGH]
				* m_componentData->shooterData->GetSpeedOffset();

	//	m_PIDs[roller].setSetpoint(speedSetpoint);
	//		AsyncPrinter::Printf("Setpoint: %.0f\n", m_PIDs[roller].getSetpoint());

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

	//	m_PIDs[roller].setInput(currentSpeedRPM);

	double openLoopInput = speedSetpoint / m_max_speed[roller];
	double normalizedError = (currentSpeedRPM - speedSetpoint)
			/ m_max_speed[roller];

	m_errorsNormalized[roller] = normalizedError;
	m_errorIntegrals[roller] *= 0.8;
	m_errorIntegrals[roller] += normalizedError * 1.0 / RobotConfig::LOOP_RATE;

	double p_gain = m_PIDs[roller].getProportionalGain();
	double i_gain = m_PIDs[roller].getIntegralGain();

	double out = openLoopInput - normalizedError * p_gain
			- m_errorIntegrals[roller] * i_gain;
	if (fabs(out) < 0.01 && currentSpeedRPM == 0.0)
	{
		m_timeoutCounter++;
		if(m_timeoutCounter >= 100)
		{
			AsyncPrinter::Printf("[ERROR] Shooter Stalling!");
			out = 0;
		}
	}
	else
	{
		m_timeoutCounter = 0;
	}
	
//	AsyncPrinter::Printf("roller %d Integral: %f\n", roller, m_errorIntegrals[roller]);
	

	//	AsyncPrinter::Printf("out: %f, openloopinput: %f, normalizederror: %f, gain: %f\n", out, openLoopInput, normalizedError, gain);

	//	double out = m_PIDs[roller].update( loopPeriod );
	//	out /= m_max_speed[roller] ; //out is a normalized voltage

	//	double maxOut = currentSpeedNormalized + m_maxNormalizedCurrent;
	//	
	//	//now lets limit the current
	//	out = Util::Min<double>(out, maxOut);

	//	if (out < 0.0)
	//		out = 0.0;// Don't do reverse power

	//	AsyncPrinter::Printf("%d: %.2f\n", out);
	double batteryAdjustment =
			(DriverStation::GetInstance()->GetBatteryVoltage()
					/ RobotConfig::MAX_VOLTAGE);

	if (batteryAdjustment < 0.2)
	{
		AsyncPrinter::Printf(
				"[ERROR] Shooter::ManageShooterWheel(): batteryAdjustment<0.1");
		return;
	}

	out /= batteryAdjustment; // batteryAdjustment is non-zero
	out = Util::Min<double>(out, 1.0);

#ifndef TALON
	if (normalizedError > 0.1)
	m_jaguars[roller]->ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	else
	m_jaguars[roller]->ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
#else
	if (normalizedError > 0.1)
		m_talons[roller]->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Brake);
	else
		m_talons[roller]->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
#endif

#ifdef OPEN_LOOP
	if (m_componentData->shooterData->ShouldLauncherBeHigh())
	{
#ifndef TALON
		m_jaguars[roller]->SetDutyCycle(m_speed_setpoints[roller][HIGH] / m_max_speed[roller]);
#else
		if (roller == OUTER)
		{
			m_talons[OUTER]->Set(m_speed_setpoints[roller][HIGH] / m_max_speed[roller]);
		}
		else
		m_jaguars[roller]->SetDutyCycle(m_speed_setpoints[roller][HIGH] / m_max_speed[roller]);
#endif
	}
	else
	{
#ifndef TALON
		m_jaguars[roller]->SetDutyCycle(m_speed_setpoints[roller][LOW] / m_max_speed[roller]);
#else
		if (roller == OUTER)
		m_talons[OUTER]->Set(m_speed_setpoints[roller][LOW] / m_max_speed[roller]);
		else
		m_jaguars[roller]->SetDutyCycle(m_speed_setpoints[roller][LOW] / m_max_speed[roller]);
#endif
	}
#else
	if (roller == OUTER)
	{
		m_outer_file << out * m_max_speed[roller] << ",";
	}
	if (roller == INNER)
	{
		m_inner_file << out * m_max_speed[roller] << ",";
	}
#ifndef TALON
	m_jaguars[roller]->SetDutyCycle(out);
#else
	m_talons[roller]->SetDutyCycle((float)out);
#endif
#endif
#ifndef TALON
	m_jaguars[roller]->SetVoltageRampRate(0.0);
#else
#endif
	//	static int e = 0;
	//	if (++e % 5 == 0)
	//		AsyncPrinter::Printf("Error %.0f\n", m_PIDs[roller].getError());

	if (roller == INNER)
		table->PutNumber("ShooterInnerOutput", out);
	else
		table->PutNumber("ShooterOuterOutput", out);
	
	if (fabs(normalizedError) < acceptableSpeedErrorNormalized[roller])
	{
		atSpeedCounter[roller]++;
		atSpeed[roller] = atSpeedCounter[roller] >= requiredCyclesAtSpeed;
		LCD::Instance()->Print(5, 19 + roller, false, "%c", '*');
	}
	else
	{
		atSpeedCounter[roller] = 0;
		atSpeed[roller] = false;
		if (normalizedError > acceptableSpeedErrorNormalized[roller])
			LCD::Instance()->Print(5, 19 + roller, false, "%c", 'v');
		if (normalizedError < -acceptableSpeedErrorNormalized[roller])
			LCD::Instance()->Print(5, 19 + roller, false, "%c", '^');
	}

	m_periods[roller] = measuredPeriod;
	m_speedsRPM[roller] = currentSpeedRPM;
}

/*
 void Shooter::ManageShooterWheel(int roller)
 {
 //TODO assert to avoid out of bounds 
 double tempSpeedRPM = (double)(m_encs[roller]->GetStopped()) ? 0.0 : (60.0 / m_encs[roller]->GetPeriod());

 
 #ifdef PATCH_BAD_SPEED_DATA
 if (tempSpeedRPM > m_max_speed[roller])
 tempSpeedRPM = m_speedsRPM[roller];
 #endif
 
 m_speedsRPM[roller] = tempSpeedRPM;
 m_periods[roller] = m_encs[roller]->GetPeriod();
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
 
 double out = m_PIDs[roller].update( 1.0 / RobotConfig::LOOP_RATE);
 out /= m_max_speed[roller] ; //out is a normalized voltage
 
 double maxOut = m_speedsRPM[roller] / m_max_speed[roller] + m_maxNormalizedCurrent;
 
 //now lets limit the current
 out = Util::Min<double>(out, maxOut);
 
 if (out < 0.0)
 out = 0.0;// Don't do rerse power
 
 //	AsyncPrinter::Printf("%d: %.2f\n", out);
 out /= (DriverStation::GetInstance()->GetBatteryVoltage() / RobotConfig::MAX_VOLTAGE);
 out = Util::Min<double>(out, 1.0);
 
 if (m_PIDs[roller].getError() < -400)
 m_jaguars[roller]->ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
 else 
 m_jaguars[roller]->ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
 
 #ifdef OPEN_LOOP
 if (m_componentData->shooterData->ShouldLauncherBeHigh())
 {
 m_jaguars[roller]->SetDutyCycle(m_speed_setpoints[roller][HIGH] / m_max_speed[roller]);
 }
 else
 {
 m_jaguars[roller]->SetDutyCycle(m_speed_setpoints[roller][LOW] / m_max_speed[roller]);
 }
 #else
 m_outer_file << out << ",";
 m_jaguars[roller]->SetDutyCycle(out);
 #endif
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
 
 }
 */

void Shooter::fubarDoDisabledPeriodic()
{
	SmarterDashboard::Instance()->EnqueueShooterMessage(
			MessageType::FRONT_SHOOTER_DATA_SPEED, 0.0f, 0.0f);
	SmarterDashboard::Instance()->EnqueueShooterMessage(
			MessageType::BACK_SHOOTER_DATA_SPEED, 0.0f, 0.0f);
	SmarterDashboard::Instance()->EnqueueShooterMessage(
			MessageType::FRONT_SHOOTER_DATA_CURRENT, 0.0f, 0.0f);
	SmarterDashboard::Instance()->EnqueueShooterMessage(
			MessageType::BACK_SHOOTER_DATA_CURRENT, 0.0f, 0.0f);

	if (m_componentData->shooterData->ShouldLauncherBeHigh())
	{
		m_angler->Set(EXTENDED);
		//		AsyncPrinter::Printf("High\n");
	}
	else
	{
		//		AsyncPrinter::Printf("Low\n");
		m_angler->Set(RETRACTED);
	}

	frisbeeExitedLastCycle = frisbeeExited = true;
	
	m_timeoutCounter = 0;
	
	table->PutNumber("ShooterInner", m_speedsRPM[INNER] / m_max_speed[INNER]);
	table->PutNumber("ShooterOuter", m_speedsRPM[OUTER] / m_max_speed[OUTER]);
	
#ifndef TALON
	m_jaguars[OUTER]->SetDutyCycle(0.0F);
	m_jaguars[INNER]->SetDutyCycle(0.0F);
#else
	m_talons[OUTER]->SetDutyCycle(0.0F);
	m_talons[INNER]->SetDutyCycle(0.0F);
#endif
	//	AsyncPrinter::Printf("Flashlight: %d\n", m_flashlight->Get());
}

void Shooter::disabledPeriodic()
{
	//	static int e = 0;
	//	static bool sw = false;
	//	if (++e % 50 == 0)
	//	{
	//		sw = !sw; 
	//	}	
	//	AsyncPrinter::Printf("fl %d\n", sw);
	//	m_flashlight->Set(sw); // Flashlight change
//	m_flashlight->Set(1); // Flashlight on when setting up the robot
//	m_flashlightPWM->SetRaw((int)m_flashlightBrightness * 255);
	m_errorIntegrals[OUTER] = 0;
	m_errorIntegrals[INNER] = 0;
	atSpeed[OUTER] = false;
	atSpeed[INNER] = false;
//	if (m_outerSensor->Get())
//	{
//		m_flashlight->Set(1);
//		m_flashlightPWM->SetRaw((int)m_flashlightBrightness * 255);
//	}
//	else
//	{
//		m_flashlight->Set(0);
//		m_flashlightPWM->SetRaw(0);
//	}

	
	fubarDoDisabledPeriodic();
}

void Shooter::Configure()

{
	ConfigManager * c = ConfigManager::Instance();
	m_max_speed[OUTER] = c->Get<double> (m_configSection, "outer_maxSpeed",
			7400);
	m_max_speed[INNER] = c->Get<double> (m_configSection, "inner_maxSpeed",
			6000);

	m_target_speed[OUTER] = c->Get<double> (m_configSection,
			"outer_speedSetpoint", 6040);
	m_target_speed[INNER] = c->Get<double> (m_configSection,
			"inner_speedSetpoint", 4400);

	m_speed_setpoints[OUTER][LOW] = c->Get<double> (m_configSection,
			"outer_highSpeedSetpoint", 5300);
	m_speed_setpoints[INNER][LOW] = c->Get<double> (m_configSection,
			"inner_highSpeedSetpoint", 4900);

	m_speed_setpoints[OUTER][HIGH] = c->Get<double> (m_configSection,
			"outer_lowSpeedSetpoint", 3100);
	m_speed_setpoints[INNER][HIGH] = c->Get<double> (m_configSection,
			"inner_lowSpeedSetpoint", 2800);
	retractWait = (int)c->Get<double> (m_configSection, "retractWaitCycles", 15);
	extendWait = (int)c->Get<double> (m_configSection, "extendWaitCycles", 10);

	//TODO: Change default values.
	requiredCyclesAtSpeed = c->Get<int> (m_configSection, "requiredCycles", 3);
	acceptableSpeedErrorNormalized[OUTER] = c->Get<double> (m_configSection,
			"outer_acceptableSpeedError", 0.01);
	acceptableSpeedErrorNormalized[INNER] = c->Get<double> (m_configSection,
			"inner_acceptableSpeedError", 0.01);

	frisbeeDetectionThreshold = c->Get<double> (m_configSection,
			"shooterSpeedDrop", 1.0 / 6.0);

	m_maxNormalizedCurrent = c->Get<double> (m_configSection,
			"normalizedCurrentThreshold", 1.0);

	requiredCyclesDown = (int)c->Get<double> (m_configSection,
			"cyclesToRetractLoader", 50);

	m_flashlightBrightness = c->Get<double> (m_configSection,
			"flashlight_brightness", 0.5);
	ConfigurePIDObject(&m_PIDs[INNER], "InnerWheelPID", 1.0);
	ConfigurePIDObject(&m_PIDs[OUTER], "OuterWheelPID", 1.0);
}

void Shooter::ConfigurePIDObject(PID *pid, std::string objName,
		bool feedForward)
{
	double p = m_config->Get<double> (Component::GetName(), objName + "_P",
			0.002);
	double i =
			m_config->Get<double> (Component::GetName(), objName + "_I", 0.0);
	double d =
			m_config->Get<double> (Component::GetName(), objName + "_D", 0.0);

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
