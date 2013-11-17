#include "LRTTalon.h"

vector<LRTTalon*> LRTTalon::talon_vector;

LRTTalon::LRTTalon(UINT32 channel, const char* name, UINT32 jumperChannel) :
	Talon(channel),
	m_name(name),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	talon_vector.push_back(this);
}

LRTTalon::LRTTalon(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel) :
	Talon(moduleNumber, channel),
	m_name(name),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
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

void LRTTalon::SetDutyCycle(float speed)
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

void LRTTalon::ConfigNeutralMode(NeutralMode mode)
{
	m_neutral = mode;
}

void LRTTalon::Update()
{
	Talon::Set(m_pwm);
	if (m_brake_jumper != NULL)
	{
		if(m_neutral == LRTSpeedController::kNeutralMode_Coast)
			m_brake_jumper->Set((UINT32)0);
		if(m_neutral == LRTSpeedController::kNeutralMode_Brake)
			m_brake_jumper->Set((UINT32)1);
				
	}
}

const char* LRTTalon::GetName()
{
	return m_name;
}
