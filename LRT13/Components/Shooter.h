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
 * @author Tony Peng, Varun Parthasarathy, Raphael Chang, Brian
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
	
	enum continuousFireState
	{
		FIRING_OFF = 0,
		RETRACT_LOADER_WAIT_FOR_LIFT = 1,
		RETRACT_LOADER_WAIT_FOR_FALL = 2,
		EXTEND_LOADER = 3
	};
	
private:
	
	void ManageShooterWheel(int roller);
	void ConfigurePIDObject(PID *pid, std::string objName, bool feedForward); //TODO make this a util
	string m_configSection;
	ofstream m_inner_file;
	ofstream m_outer_file;
	
	AsyncCANJaguar* m_jaguars[2];
	Counter* m_encs[2];

	Pneumatics* m_pneumatics;
	DigitalInput* m_proximity;
	continuousFireState m_fireState;
	
	PID m_PIDs[2];
	
	int atSpeedCounter[2];
	
	double m_max_speed[2];
	double m_target_speed[2];
	double m_speed_setpoints[2][2];
	
	bool atSpeed[2]; 
	
	int requiredCyclesAtSpeed;
	int requiredCyclesDown;
	double acceptableSpeedError[2], m_speedsRPM[2];
	double m_maxNormalizedCurrent;
	double frisbeeDetectionThreshold, lastSpeed;
	int startShotTime;
	
	int m_timer;
	int m_ticks;
	
	int m_cyclesToContinueRetracting;
	
	Notifier m_sensorProcessingNotifier;
	
	static void DeNoiseSensorEntry(void *param);
	
	void DeNoiseSensor();
	
	int m_sensorDeNoisingCycle;
#define NUM_SENSOR_VALS 5
	int m_sensorVals[NUM_SENSOR_VALS];//400/50/2 + 1
	bool m_sensorWindow;
	
	bool m_isSensorTrue;
};

#endif
