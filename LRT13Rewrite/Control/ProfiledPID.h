#ifndef PROFILED_PID_H_
#define PROFILED_PID_H_

#include "PID.h"
#include "MotionProfile.h"
#include "Timer.h"
 
class ProfiledPID : public PID
{
public:
	ProfiledPID(MotionProfile *profile, double p_gain, double i_gain, double d_gain, double ff_gain = 1.0,
			double i_decay = 0.5, bool feedforward = true);

	ProfiledPID(MotionProfile *profile);
	virtual ~ProfiledPID();

	/*!
	 * @brief updates the PID controller, call on each loop
	 * @param dt time differential
	 * @return pid output
	 */
	virtual double Update(double dt);

	/*!
	 * @brief sets the setpoint
	 * @param setpoint
	 */
	virtual void SetSetpoint(double setpoint);
	
	void SetMotionProfile(MotionProfile *profile);

private:
	double m_setpoint;
	MotionProfile *m_profile;
	Timer m_timer;
};
#endif
