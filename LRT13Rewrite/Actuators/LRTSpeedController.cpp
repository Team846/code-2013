#include "LRTSpeedController.h"
#include <math.h>
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"

LRTSpeedController::LRTSpeedController(const char *name) :
	Actuator(name),
	Loggable(name)
{
	m_encoder = NULL;
	m_timeoutSeconds = 0;
}

LRTSpeedController::~LRTSpeedController()
{
}

void LRTSpeedController::Update()
{
	if (m_encoder != NULL)
	{
		if (fabs(GetDutyCycle()) >= m_threshold && m_encoder->GetStopped())
		{
			m_timer.Start();
			if (m_timer.Get() >= m_timeoutSeconds * 1000000)
				SafetyCallback();
		}
		else
		{
			m_timer.Stop();
			m_timer.Reset();
		}
	}
	Send();
}

void LRTSpeedController::RegisterSafety(CounterBase *encoder, double timeoutSeconds, float threshold)
{
	m_encoder = encoder;
	m_timeoutSeconds = timeoutSeconds;
	m_threshold = threshold;
}

void LRTSpeedController::SafetyCallback()
{
	AsyncPrinter::Printf("[ERROR] Safety failed in LRTSpeedController: %s\n", GetName());
	SetDutyCycle(0.0);
}

float LRTSpeedController::CurrentLimit(float dutyCycle, float speed, float forwardLimit, float reverseLimit)
{
	if (speed < 0)
	{
		return -CurrentLimit(-dutyCycle, -speed, forwardLimit, reverseLimit);
	}
	// At this point speed >= 0
	if (dutyCycle > speed) // Current limit accelerating
	{
		dutyCycle = Util::Min(dutyCycle, speed + forwardLimit);
	}
	else if (dutyCycle < 0) // Current limit reversing direction
	{
		float limitedDutyCycle = -reverseLimit / (1.0 + speed); // speed >= 0 so dutyCycle < -currentLimit
		dutyCycle = Util::Max(dutyCycle, limitedDutyCycle); // Both are negative
	}
	return dutyCycle;
}

void LRTSpeedController::Log()
{
	LogToFile(GetDutyCycle(), "DutyCycle");
	LogToFile(GetNeutralMode(), "NeutralMode");
}
