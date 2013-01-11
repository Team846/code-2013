#ifndef DRIVETRAIN_H_
#define DRIVETRAIN_H_

#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "Component.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
class ESC;

class DriveEncoders;

class Drivetrain : public Component, public Configurable, public Loggable
{
public:
	Drivetrain();
	~Drivetrain();
	
	virtual void UpdateActionData(); 
	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void Configure();
	virtual void Log();
private:
	const static int POSITION = 0;
	const static int VELOCITY = 1;
	PID m_PIDs[2][2];
	
	DriveEncoders &m_driveEncoders;
	ESC *m_escs[2];
	
	void ConfigurePIDObject(PID *pid, std::string objName, bool feedForward);
	double ComputeOutput(data::drivetrain::ForwardOrTurn axis);
};

#endif
