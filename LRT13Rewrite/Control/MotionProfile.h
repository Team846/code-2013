#ifndef MOTION_PROFILE_H_
#define MOTION_PROFILE_H_

class MotionProfile
{
public:
	virtual double Update(double currentTime) = 0;
	virtual void SetSetpoint(double setpoint, double time) = 0;
};

#endif
