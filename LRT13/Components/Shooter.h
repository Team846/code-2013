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
	
private:
	string m_configSection;

	AsyncCANJaguar* m_jaguar_front;
	AsyncCANJaguar* m_jaguar_back;
	Counter* m_enc_front;
	Counter* m_enc_back;
	
	PID m_pid;

	int front_atSpeedCounter;
	int back_atSpeedCounter;
	
	float m_dutyCycle;
	
	double m_speed_front;
	double m_speed_back;
	double m_max_speed;
	
	bool front_atSpeed;
	bool back_atSpeed;
	
};

#endif
