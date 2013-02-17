#ifndef SHOOTER_H_
#define SHOOTER_H_

#include <string>

#include "Component.h"
#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Jaguar/AsyncCANJaguar.h"
#include "../ComponentData/ShooterData.h"
#include "../Pneumatics/Pneumatics.h"
#include "DigitalInput.h"

using namespace data;

/*!
 * @brief Provides control over the shooter component.
 * @author Tony Peng, Varun Parthasarathy, Raphael Chang
 */
class Shooter : public Component, public Configurable, public Loggable
{
public:
	Shooter();
	~Shooter();
	
	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void enabledPeriodic();
	virtual void disabledPeriodic();
	
	virtual void Configure();
	virtual void Log();
	
	void CheckError(int roller);
	void LimitCurrent(int roller);
	
	/*!
	 * @brief Returns the status of the current to the roller.  This can be used to determine if the roller has grabbed a frisbee.
	 * 
	 * @returns True, if the current is over the specified normal current.
	 * @returns False, if the current is under or equal to the specified normal current.
	 */
	bool IsOverCurrent();
	
private:
	string m_configSection;

	AsyncCANJaguar* m_jaguar_front;
	AsyncCANJaguar* m_jaguar_back;
	Counter* m_enc_front;
	Counter* m_enc_back;
	Pneumatics* m_pneumatics;
	DigitalInput* m_proximity;
	
	PID m_PIDs[2];
	
	double m_dutyCycleFront;
	double m_dutyCycleBack;
	
	int m_overCurrentCounter;
	int m_underCurrentCounter;

	int atSpeedCounter[2];
	
	double m_speed[2];
	double m_max_speed[2];
	
	bool atSpeed[2];
	bool frisbee_detected;
	
	int requiredCycles;
	double acceptableSpeedError[2];
	
	double maxDeltaDutyCycle, max_output[2], m_duty_cycle_delta, m_output[2];
	
};

#endif
