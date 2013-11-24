#ifndef LRT_SPEEDCONTROLLER_H_
#define LRT_SPEEDCONTROLLER_H_

#include "Actuator.h"
#include <CounterBase.h>
#include <Timer.h>

/*!
 * @brief Abstract class for various speed controllers with stall protection.
 * 
 * @author Manoj Vasishta, Raphael Chang
 */

class LRTSpeedController : public Actuator
{
public:
	typedef enum
	{
		kNeutralMode_Jumper = 0, kNeutralMode_Brake = 1, kNeutralMode_Coast = 2
	} NeutralMode;

	explicit LRTSpeedController(const char *name);
	virtual ~LRTSpeedController();
	
	void Update();
	void RegisterSafety(CounterBase *encoder, double timeoutSeconds, float threshold = 0.01);
	virtual void SafetyCallback();
	
	virtual void SetDutyCycle(float pwm) = 0;
	virtual float GetDutyCycle() = 0;
	virtual float GetHardwareValue() = 0;
	virtual void Send() = 0;
	virtual void ConfigNeutralMode(LRTSpeedController::NeutralMode mode) = 0;
	
private:
	CounterBase *m_encoder;
	double m_timeoutSeconds;
	float m_threshold;
	Timer m_timer;
};

#endif
