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
	
	AsyncCANJaguar* m_roller_jaguar;
	
	double m_dutyCycle;
	double m_roller_currentThreshold;
	
	int m_overCurrentCounter;
	int m_underCurrentCounter;
	
	int m_maxCounterDifference;
	
	string m_configSection;
	
	PID m_pid;

	int front_atSpeedCounter;
	int back_atSpeedCounter;
	
	double m_speed_front;
	double m_speed_back;
	double m_max_speed;
	
	bool front_atSpeed;
	bool back_atSpeed;
	
};

#endif
