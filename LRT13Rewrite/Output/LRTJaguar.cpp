#include "LRTJaguar.h"

vector<LRTJaguar*> LRTJaguar::jaguar_vector;

LRTJaguar::LRTJaguar(UINT32 channel, const char* name, UINT32 jumperChannel) :
	Jaguar(channel),
	m_name(name),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	jaguar_vector.push_back(this);
}

LRTJaguar::LRTJaguar(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel) :
	Jaguar(moduleNumber, channel),
	m_name(name),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	jaguar_vector.push_back(this);
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

void LRTJaguar::Update()
{
	Jaguar::Set(m_pwm);
	printf("Talon: %f\n", m_pwm);
	if (m_brake_jumper != NULL)
	{
		if(m_neutral == LRTSpeedController::kNeutralMode_Coast)
			m_brake_jumper->Set((UINT32)0);
		if(m_neutral == LRTSpeedController::kNeutralMode_Brake)
			m_brake_jumper->Set((UINT32)1);
				
	}
}

const char* LRTJaguar::GetName()
{
	return m_name;
}
