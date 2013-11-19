#include "LRTSpeedController.h"
#include <math.h>

LRTSpeedController::LRTSpeedController(const char *name) :
	Actuator(name)
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
	printf("[ERROR] Safety failed in LRTSpeedController: %s\n", GetName());
	SetDutyCycle(0.0);
}
