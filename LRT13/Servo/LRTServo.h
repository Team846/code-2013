#ifndef LRT_SERVO_H_
#define LRT_SERVO_H_

//#include "..\..\General.h"
#include "Servo.h"

class LRTServo : public Servo
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

    virtual void Set(float value);
    virtual void SetAngle(float angle);
    void SetMicroseconds(int ms);
};

#endif
