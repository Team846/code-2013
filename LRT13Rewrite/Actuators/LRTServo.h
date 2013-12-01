#ifndef LRT_SERVO_H_
#define LRT_SERVO_H_

#include "Servo.h"
#include "Actuator.h"
#include "../Logging/Loggable.h"

/*!
 * @brief Wrapper for Servos.
 */
class LRTServo : public Servo, public Actuator, public Loggable
{
public:
	typedef enum
	{
		kValue, kMicroseconds, kAngle
	} ControlMode;
	
    LRTServo(UINT32 channel, const char* name);
    ~LRTServo();

    void Update();
    
    void SetEnabled(bool enabled);
    bool IsEnabled();

    void Set(float value);
    void SetAngle(float angle);
    void SetMicroseconds(int ms);
    void SetControlMode(ControlMode mode);
    
    float Get();
    float GetHardwareValue();
    ControlMode GetControlMode();
    
    void Log();
    
private:
    ControlMode m_controlMode;
    float m_value;
    bool enabled;
    float previous_value;
    
	static const int MIN_VAL = 727;
	static const int MAX_VAL = 2252;
};

#endif
