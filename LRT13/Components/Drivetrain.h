#ifndef DRIVETRAIN_H_
#define DRIVETRAIN_H_

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "Component.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"

using namespace data;

class ESC;

class DriveEncoders;

class Drivetrain : public Component, public Configurable, public Loggable
{
public:
	Drivetrain();
	~Drivetrain();

	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void Update();
	
	virtual void Configure();
	virtual void Log();
private:
	const static int POSITION = 0;
	const static int VELOCITY = 1;
	PID m_PIDs[2][2];
	
	DriveEncoders &m_driveEncoders;
	ESC *m_escs[2];
	
	bool m_isEnabled;
	
	void ConfigurePIDObject(PID *pid, std::string objName, bool feedForward);
	double ComputeOutput(data::drivetrain::ForwardOrTurn axis);
};

#endif
