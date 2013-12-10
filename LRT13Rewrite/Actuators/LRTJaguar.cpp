#include "LRTJaguar.h"

vector<LRTJaguar*> LRTJaguar::jaguar_vector;

LRTJaguar::LRTJaguar(UINT32 channel, const char* name, UINT32 jumperChannel) :
	Jaguar(channel),
	LRTSpeedController(("LRTJaguar" + std::string(name)).c_str()),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	jaguar_vector.push_back(this);
	
	printf("Constructed LRTJaguar %s on channel %2d\n", name, channel);
}

LRTJaguar::LRTJaguar(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel) :
	Jaguar(moduleNumber, channel),
	LRTSpeedController(("LRTJaguar" + std::string(name)).c_str()),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	jaguar_vector.push_back(this);

	printf("Constructed LRTJaguar %s on channel %2d\n", name, channel);
}

LRTJaguar::~LRTJaguar()
{
	if (m_brake_jumper != NULL)
	{
		delete m_brake_jumper;
		m_brake_jumper = NULL;
	}
}

void LRTJaguar::SetDutyCycle(float speed)
{
	m_pwm = speed;
}

float LRTJaguar::GetDutyCycle()
{
	return m_pwm;
}

float LRTJaguar::GetHardwareValue()
{
	return Jaguar::Get();
}

void LRTJaguar::Set(float speed)
{
	printf("[WARNING] Calling Set() in LRTTalon: %s, use SetDutyCycle() instead.\n;", GetName());
	SetDutyCycle(speed);
}

float LRTJaguar::Get()
{
	return m_pwm;
}

void LRTJaguar::Disable()
{
	m_pwm = kPwmDisabled;
}

void LRTJaguar::PIDWrite(float output) 
{
	Set(output);
}

void LRTJaguar::ConfigNeutralMode(NeutralMode mode)
{
	m_neutral = mode;
}

LRTJaguar::NeutralMode LRTJaguar::GetNeutralMode()
{
	return m_neutral;
}

void LRTJaguar::Send()
{
	Jaguar::Set(m_pwm);
	if (m_brake_jumper != NULL)
	{
		if(m_neutral == LRTSpeedController::kNeutralMode_Coast)
			m_brake_jumper->Set((UINT32)0);
		if(m_neutral == LRTSpeedController::kNeutralMode_Brake)
			m_brake_jumper->Set((UINT32)1);
				
	}
}
