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
	
private:
	
	void ManageShooterWheel(int roller);
	string m_configSection;

	AsyncCANJaguar* m_jaguars[2];
	Counter* m_encs[2];

	Pneumatics* m_pneumatics;
	DigitalInput* m_proximity;
	
	PID m_PIDs[2];
	
	int atSpeedCounter[2];
	
	double m_max_speed[2];
	
	bool atSpeed[2]; 
	
	int requiredCyclesAtSpeed;
	double acceptableSpeedError[2];
	double m_maxNormalizedCurrent;
	double frisbeeDetectionThreshold;
};

#endif
