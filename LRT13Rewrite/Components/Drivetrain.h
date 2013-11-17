#ifndef DRIVETRAIN_H_
#define DRIVETRAIN_H_

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "Component.h"
#include "../Utils/PID.h"
#include "../Utils/ProfiledPID.h"
#include "../Utils/TrapezoidProfile.h"
#include "../ComponentData/DrivetrainData.h"

using namespace data;

class ESC;

class DriveEncoders;

/*!
 * @brief Provides control over the drivetrain component.
 * @author Brian Axelrod, Tony Peng, Raphael Chang
 */
class Drivetrain : public Component, public Configurable, public Loggable
{
public:
	Drivetrain();
	~Drivetrain();

	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void enabledPeriodic();
	virtual void disabledPeriodic();
	
	virtual void Configure();
	virtual void Log();
private:
	const static int POSITION = 0;
	const static int VELOCITY = 1;
	PID m_PIDs[2][2];
	TrapezoidProfile *m_profiles[2];
	ProfiledPID* m_profiled[2];
	double m_timeToMax[2];
	
	DriveEncoders* m_driveEncoders;
	ESC *m_escs[4];
	
	double m_errorThreshold;
	
	double m_scale;
	double m_arcGain;

	NetworkTable *table;
	ofstream m_log_file;
	
	double lastSpeed[4];
	
	void ConfigurePIDObject(PID *pid, std::string objName, bool feedForward);
	void ConfigureForwardCurrentLimit();
	void ConfigureReverseCurrentLimit();
	double ComputeOutput(data::drivetrain::ForwardOrTurn axis);
	double ComputeSide(data::drivetrain::Side side, double forward, double turn);
};

#endif
