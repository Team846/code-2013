#ifndef SHOOTER_H_
#define SHOOTER_H_

#include <string>

#include "Component.h"
#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../SpeedController/AsyncCANJaguar.h"
#include "../SpeedController/LRTTalon.h"
#include "../ComponentData/ShooterData.h"
#include "../Pneumatics/Pneumatics.h"
#include "DigitalInput.h"
#include "../Network/MessageType.h"
#include "../Utils/SmarterDashboard.h"
//#include "Relay.h"
#include "DigitalOutput.h"
#include "NetworkTables/NetworkTable.h"

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
	void fubarDoDisabledPeriodic();
	
	void ManageShooterWheel(int roller);
	void ConfigurePIDObject(PID *pid, std::string objName, bool feedForward); //TODO make this a util
	string m_configSection;
	ofstream m_inner_file;
	ofstream m_outer_file;
	
	AsyncCANJaguar* m_jaguars[2];
	Counter* m_encs[2];
	LRTTalon* m_talon;

	Pneumatics* m_pneumatics;
	DigitalInput* m_proximity;
	continuousFireState m_fireState;
	
	PID m_PIDs[2];
	double m_errorsNormalized[2];
	double m_errorIntegrals[2];
	
	int atSpeedCounter[2];
	
	double m_max_speed[2];
	double m_target_speed[2];
	double m_speed_setpoints[2][2];
	
	bool atSpeed[2]; 
	
	int requiredCyclesAtSpeed;
	int requiredCyclesDown;
	double acceptableSpeedErrorNormalized[2], m_speedsRPM[2], m_periods[2];
	double m_maxNormalizedCurrent;
	double frisbeeDetectionThreshold;
	int startShotTime;
	
	int m_timer;
	int m_ticks;
	
	int m_cyclesToContinueRetracting;
	
	Notifier m_sensorProcessingNotifier;
	
	DigitalOutput* m_flashlight; // Flashlight change
	
	NetworkTable *table;
	
	static void DeNoiseSensorEntry(void *param);
	
	void DeNoiseSensor();
	
	int m_sensorDeNoisingCycle;
	int firingWaitTicks;
	int retractWait;
	int extendWait;
#define NUM_SENSOR_VALS 5
	DigitalInput* m_outerSensor;
	int m_sensorVals[NUM_SENSOR_VALS];//400/50/2 + 1
	bool m_sensorWindow;
	
	bool m_isSensorTrue;
	
	bool m_sensorOK;
	int m_sensorStableTime;
	
	bool frisbeeExited;
	bool frisbeeExitedLastCycle;
	
	int m_timeoutCounter;
};

#endif
