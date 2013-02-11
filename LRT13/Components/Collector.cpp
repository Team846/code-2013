#include "Collector.h"
#include "../ComponentData/CollectorData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

Collector::Collector()
: Component("Collector", DriverStationConfig::DigitalIns::COLLECTOR, true),
  m_configSection("collector")
{
	m_jaguar = new AsyncCANJaguar(RobotConfig::CAN::COLLECTOR, "Collector");
	m_proximityA = new DigitalInput(RobotConfig::Digital::PROXIMITY_A);
	m_proximityB = new DigitalInput(RobotConfig::Digital::PROXIMITY_B);
}

Collector::~Collector()
{
	DELETE(m_proximityA);
	DELETE(m_proximityB);
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
	m_jaguar->SetDutyCycle(m_dutyCycle);
	
	//Increments Orientation + Error Counters
	
	if (m_proximityA->Get() == 1 && m_proximityB->Get() == 0)
	{
		m_upCount++;
	}
	else
	{
		m_upCount = 0;
	}
	if (m_proximityA->Get() == 0 && m_proximityB->Get() == 1)
	{
		m_downCount++;
	}
	else
	{
		m_downCount = 0;
	}
	if (m_proximityA->Get() == 1 && m_proximityB->Get() == 1)
	{
		m_errCount++;
	}
	else
	{
		m_errCount = 0;
	}
	
	//Operating on assupmtion that frisbees aren't flush against each other
	if(m_downCount == SAMPLES_THRESHOLD || m_upCount == SAMPLES_THRESHOLD)
	{
		
		RobotData::IncrementFrisbeeCounter(RobotData::UP);	
	}
	if(m_errCount == SAMPLES_THRESHOLD)
	{
		; //Add ErrLog; Extreme Noise or Weird Sensor Config
	}
}

void Collector::disabledPeriodic()
{
	m_jaguar->SetDutyCycle(0.0F);
}

void Collector::Configure()
{
	m_dutyCycle = ConfigManager::Instance()->Get<float> (m_configSection, "speed", 0.3F);
}

void Collector::Log()
{

}
