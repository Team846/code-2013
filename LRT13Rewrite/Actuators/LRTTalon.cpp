#include "LRTTalon.h"

vector<LRTTalon*> LRTTalon::talon_vector;

LRTTalon::LRTTalon(UINT32 channel, const char* name, UINT32 jumperChannel) :
	Talon(channel),
	LRTSpeedController(("LRTTalon" + std::string(name)).c_str()),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	talon_vector.push_back(this);
	
	printf("Constructed LRTTalon %s on channel %2d\n", name, channel);
}

LRTTalon::LRTTalon(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel) :
	Talon(moduleNumber, channel),
	LRTSpeedController(("LRTTalon" + std::string(name)).c_str()),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	talon_vector.push_back(this);
	
	printf("Constructed LRTTalon %s on channel %2d\n", name, channel);
}

LRTTalon::~LRTTalon()
{
	delete m_brake_jumper;
	m_brake_jumper = NULL;
}

void LRTTalon::SetDutyCycle(float speed)
{
	m_pwm = speed;
}

float LRTTalon::GetDutyCycle()
{
	return m_pwm;
}

float LRTTalon::GetHardwareValue()
{
	return Talon::Get();
}

void LRTTalon::Set(float speed)
{
	printf("[WARNING] Calling Set() in LRTTalon: %s, use SetDutyCycle() instead.\n;", GetName());
	SetDutyCycle(speed);
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
	SetDutyCycle(output);
}

void LRTTalon::ConfigNeutralMode(NeutralMode mode)
{
	m_neutral = mode;
}

LRTTalon::NeutralMode LRTTalon::GetNeutralMode()
{
	return m_neutral;
}

void LRTTalon::Send()
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
