#ifndef TRAPEZOID_PROFILE_H_
#define TRAPEZOID_PROFILE_H_

#include "MotionProfile.h"

class TrapezoidProfile : public MotionProfile
{
public:
	TrapezoidProfile(double maxV, double timeToMaxV);
	
	virtual double Update(double currentTime);
	virtual void SetSetpoint(double setpoint, double time);
	void updateValues(double maxV, double timeToMaxV);
	double getVelocity();
	double getOutput();
	
private:
	double m_maxV;
	double m_timeToMaxV;
	double m_accel;
	double m_startingTime;
	double m_setpoint;
	double m_output;
	double m_timeToMax;
	double m_timeFromMax;
	double m_timeTotal;
	double m_lastTime;
	double m_velocity;
};

#endif
