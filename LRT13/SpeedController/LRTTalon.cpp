#include "LRTTalon.h"

vector<LRTTalon*> LRTTalon::talon_vector;

LRTTalon::LRTTalon(UINT32 channel, const char* name, UINT32 jumperChannel) :
	Talon(channel),
	m_name(name),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	talon_vector.push_back(this);
}

LRTTalon::LRTTalon(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel) :
	Talon(moduleNumber, channel),
	m_name(name),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	talon_vector.push_back(this);
}

LRTTalon::~LRTTalon()
{
	if (m_brake_jumper != NULL)
	{
		delete m_brake_jumper;
		m_brake_jumper = NULL;
	}
}

void LRTTalon::Set(float speed)
{
	m_pwm = speed;
}

float LRTTalon::Get()
{
	return m_pwm;
}

void LRTTalon::Disable()
{
	m_pwm = kPwmDisabled;
}

void LRTTalon::PIDWrite(float output) 
{
	Set(output);
}

void LRTTalon::SetNeutralMode(NeutralMode mode)
{
	m_neutral = mode;
}

void LRTTalon::Update()
{
	Talon::Set(m_pwm);
	if (m_brake_jumper != NULL)
		m_brake_jumper->Set((UINT32)m_neutral);
}

const char* LRTTalon::GetName()
{
	return m_name;
}
