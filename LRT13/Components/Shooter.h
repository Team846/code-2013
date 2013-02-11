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
 * @author Tony Peng
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

	AsyncCANJaguar* m_jaguar;
	Counter* m_enc;
	
	PID m_pid;

	int wrongSpeedCounter;
	
	float m_dutyCycle;
	
	double m_speed;
	double m_max_speed;

	data::shooter::shooterConfiguration m_shooterConfiguration;
	
};

#endif
