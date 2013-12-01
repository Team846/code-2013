#include "LRTServo.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"

LRTServo::LRTServo(UINT32 channel, const char* name)
    : Servo(channel)
	, Actuator(name)
	, Loggable("Servo" + std::string(name))
    , m_controlMode(kValue)
	, m_value(0.0)
    , enabled(false)
    , previous_value(999.0)
{
    printf("Created LRTServo %s on channel %d\n", name, channel);
}

LRTServo::~LRTServo()
{
}

void LRTServo::Update()
{
	if (enabled)
	{
		switch(m_controlMode)
		{
		case kValue:
	        Servo::Set(m_value);
			break;
		case kMicroseconds:
	    	float val = (float)(m_value - MIN_VAL) / (MAX_VAL - MIN_VAL);
	        Servo::Set(val);
			break;
		case kAngle:
	        Servo::SetAngle(m_value);
			break;
		}
	}
	else
        this->SetOffline();
}

void LRTServo::SetEnabled(bool enabled)
{
    this->enabled = enabled;
}

bool LRTServo::IsEnabled()
{
    return enabled;
}

void LRTServo::Set(float value)
{
	m_controlMode = kValue;
	m_value = value;
}

void LRTServo::SetMicroseconds(int ms) 
{
	m_controlMode = kMicroseconds;
	m_value = Util::Clamp<int>(ms, MIN_VAL, MAX_VAL);
}

void LRTServo::SetAngle(float angle)
{
	m_controlMode = kAngle;
	m_value = angle;
}

void LRTServo::SetControlMode(LRTServo::ControlMode mode)
{
	m_controlMode = mode;
}

float LRTServo::Get()
{
	return m_value;
}

float LRTServo::GetHardwareValue()
{
	return Servo::Get();
}

LRTServo::ControlMode LRTServo::GetControlMode()
{
	return m_controlMode;
}

void LRTServo::Log()
{
	LogToFile(&m_controlMode, "ControlMode");
	LogToFile(&m_value, "Value");
}
