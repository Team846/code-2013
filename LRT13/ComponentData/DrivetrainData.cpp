#include "DrivetrainData.h"
#include "../Sensors/DriveEncoders.h"
#include <math.h>


using namespace data;
using namespace data::drivetrain;

DrivetrainData::DrivetrainData()
{
	m_driveEncoders = DriveEncoders::GetInstance();
	
	// these arrays are statically allocated and thus cannot be deallocated... -tp
	// These arrays are not statically allocated shoudl still nto be deallocated since they will be cleaned up upon destruction. 
	
	memset(m_controlModes, OPEN_LOOP, sizeof(m_controlModes));
	memset(m_desiredRates, 0, sizeof(m_desiredRates));
	memset(m_positionSetpoints, 0, sizeof(m_positionSetpoints));
	memset(m_maxSpeeds, 0, sizeof(m_maxSpeeds));
}

DrivetrainData::~DrivetrainData()
{
}

ControlMode DrivetrainData::getControlMode(ForwardOrTurn axis)
{
	return m_controlModes[axis];
}

void DrivetrainData::setOpenLoopOutput(ForwardOrTurn axis, double setpoint)
{
	m_desiredOpenLoopOutputs[axis] = setpoint;
}

void DrivetrainData::setVelocitySetpoint(ForwardOrTurn axis, double setpoint)
{
	m_desiredRates[axis] = setpoint;
}

void DrivetrainData::setRelativePositionSetpoint(ForwardOrTurn axis,
		double setpoint, double maxspeed)
{
	m_positionSetpoints[axis] = setpoint + getCurrentPos(axis);
	m_maxSpeeds[axis] = maxspeed;
}

void DrivetrainData::DebugPrintPosition(ForwardOrTurn axis)
{
	AsyncPrinter::Printf("Curr: %.4f, desired %.4f\n",getCurrentPos(axis), m_positionSetpoints[axis] );
}

double DrivetrainData::getCurrentPos(ForwardOrTurn axis)
{
	double currentPos;
	if (axis == FORWARD)
		currentPos = m_driveEncoders->getRobotDist();
	else if (axis == TURN)
		currentPos = m_driveEncoders->getTurnAngle();
	return currentPos;
}

void DrivetrainData::setMaxPositionControlSpeed(ForwardOrTurn axis, double maxSpeed)
{
	m_maxSpeeds[axis] = maxSpeed;
}

void DrivetrainData::setControlMode(ForwardOrTurn axis, ControlMode control)
{
	m_controlModes[axis] = control;
}

SEM_ID DrivetrainData::createPositionOperationSemaphore(ForwardOrTurn axis,
		double errorThreshold)
{
	drivetrainOpSem sem;
	sem.sem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	semTake(sem.sem, WAIT_FOREVER);
	sem.axis = axis;
	sem.errorThreshold = errorThreshold;
	operationSems.push_back(sem);
	return sem.sem;
}

void DrivetrainData::cleanWaitForSem(SEM_ID sem)
{
	semTake(sem, WAIT_FOREVER);
	semDelete(sem);
}
bool DrivetrainData::cleanWaitForSem(SEM_ID sem, double timeout)
{
	STATUS s = semTake(sem, (int) (sysClkRateGet() * timeout));
	if (s == OK)
	{
		semDelete(sem);
		return true;
	}
	else
	{
		return false;
	}
}

void DrivetrainData::serviceOperationSemaphores()
{
	list<drivetrainOpSem>::iterator iterator;
	for (iterator = operationSems.begin(); iterator != operationSems.end(); ++iterator) 
	{
		double error = getRelativePositionSetpoint(iterator->axis);
		if (fabs(error) < iterator->errorThreshold)
		{
			semGive(iterator->sem);
			iterator = operationSems.erase(iterator);
			iterator--; //it is up to the auton routine to delete 
		}
	}
}



bool DrivetrainData::isDesiredPositionOperationComplete(ForwardOrTurn axis,
		double errorThreshold)
{
	return false;
}

double DrivetrainData::getOpenLoopOutput(ForwardOrTurn axis)
{
	return m_desiredOpenLoopOutputs[axis];
}
double DrivetrainData::getVelocitySetpoint(ForwardOrTurn axis)
{
	return m_desiredRates[axis];
}
double DrivetrainData::getRelativePositionSetpoint(ForwardOrTurn axis)
{
	return m_positionSetpoints[axis] - getCurrentPos(axis);
}
double DrivetrainData::getPositionControlMaxSpeed(ForwardOrTurn axis)
{
	return m_maxSpeeds[axis];
}

