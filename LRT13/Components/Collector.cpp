#include "Collector.h"
#include "../ComponentData/CollectorData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../ComponentData/ShooterData.h"
#include "../ComponentData/ClimberData.h"

#define SLOW_DOWN

Collector::Collector()
: Component("Collector", DriverStationConfig::DigitalIns::COLLECTOR, true),
  m_configSection("collector")
{
	m_jaguar = new AsyncCANJaguar(RobotConfig::CAN::COLLECTOR, "Collector");
	m_proximity = new DigitalInput(RobotConfig::Digital::PROXIMITY_COLLECTOR);
	m_pneumatics = Pneumatics::Instance();
	m_dutyCycle = 0.0;
	m_count = 0;
	m_samplesThreshold = 0;
	m_overflowWait = 0;
	m_reverse = false;
	m_lastReverseState = false;
	m_timer = 0;
	m_lastStateWasUp = true;
	m_jaguar->setCollectionFlags(AsyncCANJaguar::OUTCURR);
	m_slowDownTicks = 0;
	m_slowDown = false;
	m_currentCount = 0;
	Configure();
}

Collector::~Collector()
{
	DELETE(m_proximity);
	DELETE(m_jaguar);
}

void Collector::onEnable()
{
	
}

void Collector::onDisable()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Collector::enabledPeriodic()
{
	if (m_componentData->collectorData->shouldStupidMoveDown)
	{
		m_pneumatics->setCollector(true);
		m_jaguar->SetDutyCycle(0.0);
		return;
	}
//	if (m_componentData->climberData->getCurrentState() != climber::NOTHING)
//		return;
	if (m_componentData->collectorData->ShouldRunRollers())
	{
#ifdef SLOW_DOWN
		double current = m_jaguar->GetOutputCurrent();
		if (current > 14.0 && current < 20.0)
		{
			m_currentCount++;
		}
		else
		{
			m_currentCount = 0;
		}
		if (m_currentCount > 5)
			m_slowDown = true;
		if (m_slowDown)
		{
			m_jaguar->SetDutyCycle(m_slowDownAmount * m_componentData->collectorData->RollerDirection());
			m_slowDownTicks++;
		}
		else
		{
			m_jaguar->SetDutyCycle(m_dutyCycle * m_componentData->collectorData->RollerDirection());
		}
		if (m_slowDownTicks > m_slowDownTime)
		{
			m_slowDownTicks = 0;
			m_slowDown = false;
		}
#else
		m_jaguar->SetDutyCycle(m_dutyCycle * m_componentData->collectorData->RollerDirection());
#endif
//		if ((unsigned int)RobotData::GetFrisbeeCounter() < RobotConfig::MAX_GAME_PIECES)
//		{
//			m_jaguar->SetDutyCycle(m_dutyCycle);
//			m_reverse = false;
//		}
//		else if ((unsigned int)RobotData::GetFrisbeeCounter() >= RobotConfig::MAX_GAME_PIECES && m_overflowWait < m_overflowWaitThreshold)
//		{
//			m_jaguar->SetDutyCycle(m_dutyCycle);
//			m_overflowWait++;
//			m_reverse = false;
//		}
//		else
//		{
//			m_overflowWait = 0;
//			m_jaguar->SetDutyCycle(-m_dutyCycle);
//			m_reverse = true;
//		}
	}
	else
	{
		m_jaguar->SetDutyCycle(0.0);
	}
	
	if (m_lastStateWasUp != m_componentData->collectorData->IsUp())//change of state
	{
		if (m_componentData->collectorData->IsDown())
		{
			//going down is easy for now
			m_pneumatics->setCollector(true);
			m_componentData->shooterData->SetLauncherAngleLow();
			m_timer = m_time_before_extend;
		}
		else
		{
			AsyncPrinter::Printf("Started going up\n");
			m_componentData->shooterData->SetLauncherAngleLow();
			m_timer = m_time_before_retract;
//			m_pneumatics->setCollector(false);
		}
		
	}
	else if (m_componentData->collectorData->IsDown())
	{
		if (m_timer > 0)
		{
			m_timer--;
		}
		else
		{
			m_componentData->shooterData->SetLauncherAngleHigh();
		}
		m_pneumatics->setCollector(true);
	}
	else
	{
		if (m_timer > 0)
		{
			m_timer--;
		}
		else
		{
			m_pneumatics->setCollector(false);
		}
	}
	m_lastStateWasUp = m_componentData->collectorData->IsUp();
//	if (m_proximity->Get() == 0)
//	{
//		m_count++;
//	}
//	else
//	{
//		m_count = 0;
//	}
//	
//	if (m_count == m_samplesThreshold && m_lastReverseState == m_reverse) // If rollers direction is changed when a frisbee is first detected, don't do anything because frisbee count is not actually changing (anything that was coming in will be spit back out, and vice versa).
//	{
//		if (!m_reverse)
//		{
//			m_componentData->shooterData->IncrementFrisbeeCounter();
//			RobotData::IncrementFrisbeeCounter();
//		}
//		else
//		{
//			m_componentData->shooterData->DecrementFrisbeeCounter();
//			RobotData::DecrementFrisbeeCounter();
//		}
//	}
//	if (m_count > m_samplesThreshold && m_lastReverseState != m_reverse) // Change in roller direction when frisbee is on sensor, undo the change made
//	{
//		if (!m_reverse)
//		{
//			m_componentData->shooterData->IncrementFrisbeeCounter();
//			RobotData::IncrementFrisbeeCounter();
//		}
//		else
//		{
//			m_componentData->shooterData->DecrementFrisbeeCounter();
//			RobotData::DecrementFrisbeeCounter();
//		}
//	}
//	m_lastReverseState = m_reverse;
	
	m_componentData->collectorData->SetExtendTime(m_time_before_extend);
	m_componentData->collectorData->SetRetractTime(m_time_before_retract);
}

void Collector::disabledPeriodic()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Collector::Configure()
{
	m_dutyCycle = ConfigManager::Instance()->Get<float> (m_configSection, "speed", 1.0F);
	m_samplesThreshold = ConfigManager::Instance()->Get<int> (m_configSection, "samplesThreshold", 4);
	m_overflowWaitThreshold = ConfigManager::Instance()->Get<int> (m_configSection, "overflowWaitCycles", 50);
	m_time_before_retract = ConfigManager::Instance()->Get<int>(m_configSection, "cyclesBeforeRetract", 15);
	m_time_before_extend = ConfigManager::Instance()->Get<int>(m_configSection, "cyclesBeforeExtend", 15);
	m_slowDownTime = ConfigManager::Instance()->Get<int>(m_configSection, "slowDownTicks", 50);
	m_slowDownAmount = ConfigManager::Instance()->Get<float>(m_configSection, "slowDownDutyCycle", 0.5);
}

void Collector::Log()
{
	
}
