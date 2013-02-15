#include "PTO.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../Config/ConfigManager.h"

PTO::PTO()
: Component("PTO", -1, true),
m_configSection("pto")
{
	m_leftServo = new Servo(RobotConfig::PWM::LEFT_PTO_SERVO);
	m_rightServo = new Servo(RobotConfig::PWM::RIGHT_PTO_SERVO);
}

PTO::~PTO()
{
	
}

void PTO::onEnable()
{
	
}

void PTO::onDisable()
{
	
}
	
void PTO::enabledPeriodic()
{
	if (m_componentData->ptoData->IsEngaged())
	{
		m_leftServo->Set(m_leftEngagedServoValue);
		m_rightServo->Set(m_rightEngagedServoValue);
	}
	else
	{
		m_leftServo->Set(m_leftDisengagedServoValue);
		m_rightServo->Set(m_rightDisengagedServoValue);
	}
}

void PTO::disabledPeriodic()
{
	
}

void PTO::Configure()
{
	ConfigManager* c = ConfigManager::Instance();
	m_leftEngagedServoValue = c->Get<int>(m_configSection, "leftEngagedServoValue", 1514);
	m_leftDisengagedServoValue = c->Get<int>(m_configSection, "leftDisengagedGearServoValue", 905);
	m_rightEngagedServoValue = c->Get<int>(m_configSection, "rightEngagedServoValue", 1514);
	m_rightDisengagedServoValue = c->Get<int>(m_configSection, "rightDisengagedServoValue", 905);
}

void PTO::Log()
{
	
}
