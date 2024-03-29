#include "DrivetrainData.h"
#include "../Sensors/DriveEncoders.h"
#include <math.h>

using namespace data;
using namespace data::drivetrain;

DrivetrainData::DrivetrainData()
{
	m_driveEncoders = DriveEncoders::GetInstance();

	m_overrideCurrentLimitForward = false;
	m_overrideCurrentLimitReverse = false;
	m_currentLimitForward = 0.5;
	m_currentLimitReverse = 0.5;
	
	memset(m_controlModes, OPEN_LOOP, sizeof(m_controlModes));
	memset(m_desiredRates, 0, sizeof(m_desiredRates));
	memset(m_positionSetpoints, 0, sizeof(m_positionSetpoints));
	memset(m_maxSpeeds, 0, sizeof(m_maxSpeeds));
	memset(m_positionSetpointChanged, 0, sizeof(m_positionSetpointChanged));
	memset(m_positionStart, 0, sizeof(m_positionStart));
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
		double setpoint, double maxspeed, bool fromLastSetpoint)
{
	m_syncArc = false;
	if (fromLastSetpoint)
	{
		m_positionSetpoints[axis] += setpoint;
	}
	else
	{
		m_positionSetpoints[axis] = setpoint + getCurrentPos(axis);
	}
	m_maxSpeeds[axis] = maxspeed;
	m_positionSetpointChanged[axis] = true;
	m_positionStart[axis] = getCurrentPos(axis);
}


bool DrivetrainData::isPositionSetpointChanged(ForwardOrTurn axis)
{
	return m_positionSetpointChanged[axis];
}
void DrivetrainData::setPositionSetpointChanged(ForwardOrTurn axis, bool changed)
{
	m_positionSetpointChanged[axis] = changed;
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

void DrivetrainData::setZeroHeading()
{
	m_zeroHeading = fmod(m_driveEncoders->getTurnAngle(), 360);
	if (m_zeroHeading < 0)
		m_zeroHeading += 360;
}

void DrivetrainData::zeroLastPositionSetpoint(ForwardOrTurn axis)
{
	m_positionSetpoints[axis] = getCurrentPos(axis);
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
double DrivetrainData::getAbsolutePositionSetpoint(ForwardOrTurn axis)
{
	return m_positionSetpoints[axis];
}
double DrivetrainData::getPositionControlMaxSpeed(ForwardOrTurn axis)
{
	return m_maxSpeeds[axis];
}

double DrivetrainData::getPositionControlStartingPosition(ForwardOrTurn axis)
{
	return m_positionStart[axis];
}

float DrivetrainData::getCurrentHeading()
{
	float m_currentHeading = fmod(m_driveEncoders->getTurnAngle(), 720);
	if (m_currentHeading < 0)
		m_currentHeading += 360;
	return m_currentHeading - m_zeroHeading;
}

void DrivetrainData::syncArc()
{
	m_syncArc = true;
}

bool DrivetrainData::syncingArc()
{
	return m_syncArc;
}

void DrivetrainData::overrideForwardCurrentLimit(float limit)
{
	m_overrideCurrentLimitForward = true;
	if (limit < 0)
		limit = 0;
	else if (limit > 1.0)
		limit = 1.0;
	m_currentLimitForward = limit;
}

void DrivetrainData::overrideReverseCurrentLimit(float limit)
{
	m_overrideCurrentLimitReverse = true;
	if (limit < 0)
		limit = 0;
	else if (limit > 1.0)
		limit = 1.0;
	m_currentLimitReverse = limit;
}

float DrivetrainData::getForwardCurrentLimit()
{
	m_overrideCurrentLimitForward = false;
	return m_currentLimitForward;
}

float DrivetrainData::getReverseCurrentLimit()
{
	m_overrideCurrentLimitReverse = false;
	return m_currentLimitReverse;
}

bool DrivetrainData::shouldOverrideForwardCurrentLimit()
{
	return m_overrideCurrentLimitForward;
}

bool DrivetrainData::shouldOverrideReverseCurrentLimit()
{
	return m_overrideCurrentLimitReverse;
}
