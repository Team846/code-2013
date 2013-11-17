#ifndef MOTION_PROFILE_H_
#define MOTION_PROFILE_H_

class MotionProfile
{
public:
	virtual double update(double currentTime) = 0;
	virtual void setSetpoint(double setpoint, double time) = 0;
};

#endif
