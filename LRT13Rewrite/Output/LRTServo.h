#ifndef LRT_SERVO_H_
#define LRT_SERVO_H_

#include "Servo.h"
#include "Output.h"

class LRTServo : public Servo, public Output
{
private:
    bool enabled;
    float previous_value_; //use for detecting changes.

public:
    char* m_name; //user supplied name of servo for diagnostics
    LRTServo(UINT32 channel, char* name);
    ~LRTServo();

    void SetEnabled(bool enabled);
    bool IsEnabled();

    void Set(float value);
    void SetAngle(float angle);
    void SetMicroseconds(int ms);
};

#endif
