#ifndef LRT_SPEEDCONTROLLER_H_
#define LRT_SPEEDCONTROLLER_H_

#include "Actuator.h"
#include <CounterBase.h>
#include <Timer.h>
#include "../Logging/Loggable.h"

/*!
 * @brief Abstract class for various speed controllers with stall protection.
 * 
 * @author Manoj Vasishta, Raphael Chang
 */
class LRTSpeedController : public Actuator, public Loggable
{
public:
	typedef enum
	{
		kNeutralMode_Jumper = 0, kNeutralMode_Brake = 1, kNeutralMode_Coast = 2
	} NeutralMode;

	explicit LRTSpeedController(const char *name);
	virtual ~LRTSpeedController();
	
	/*!
	 * @brief Checks safety and sends the saved values to the speed controllers.
	 */
	void Update();
	
	/*!
	 * @brief Registers an encoder with this speed controller for stall safety.
	 * @param encoder the encoder to check for stalling
	 * @param timeoutSeconds how long the motor is allowed to stall before turned off, in seconds
	 * @param threshold the stall threshold
	 */
	void RegisterSafety(CounterBase *encoder, double timeoutSeconds, float threshold = 0.01);
	
	/*!
	 * @brief Called when safety fails on this speed controller.
	 */
	virtual void SafetyCallback();
	
	/*!
	 * @brief Sets the duty cycle of this speed controller.
	 * @param pwm the duty cycle to set
	 */
	virtual void SetDutyCycle(float pwm) = 0;
	
	/*!
	 * @brief Returns the last set duty cycle of this speed controller.
	 * @return the last set duty cycle
	 */
	virtual float GetDutyCycle() = 0;
	
	/*!
	 * @brief Returns the last duty cycle sent to the speed controller.
	 * @return the last duty cycle sent
	 */
	virtual float GetHardwareValue() = 0;
	
	/*!
	 * @brief Sends the saved values to the speed controllers.
	 */
	virtual void Send() = 0;
	
	/*!
	 * @brief Sets the neutral mode (brake or coast).
	 * @param mode the neutral mode (brake or coast)
	 */
	virtual void ConfigNeutralMode(LRTSpeedController::NeutralMode mode) = 0;
	
	/*!
	 * @brief Gets the neutral mode (brake or coast).
	 * @return the neutral mode (brake or coast)
	 */
	virtual NeutralMode GetNeutralMode() = 0;
	
	/*!
	 * @brief Calculates a duty cycle with current limiting applied
	 * @param dutyCycle the input duty cycle to limit
	 * @param speed the normalized current speed of the motor
	 * @param forwardLimit the current limit for acceleration
	 * @param reverseLimit the current limit for reversing direction
	 * @return the duty cycle with current limiting applied
	 */
	static float CurrentLimit(float dutyCycle, float speed, float forwardLimit, float reverseLimit);
	
	virtual void Log();
	
private:
	CounterBase *m_encoder;
	double m_timeoutSeconds;
	float m_threshold;
	Timer m_timer;
};

#endif
