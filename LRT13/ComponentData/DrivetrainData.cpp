#include "DrivetrainData.h"
#include "../Sensors/DriveEncoders.h"

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
	m_positionFwdSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	m_positionTurnSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
}

DrivetrainData::~DrivetrainData()
{
	semDelete(m_positionFwdSemaphore);
	semDelete(m_positionTurnSemaphore);
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

SEM_ID DrivetrainData::positionOperationSemaphore(ForwardOrTurn axis,
		double errorThreshold)
{
	return axis == drivetrain::FORWARD ? m_positionFwdSemaphore : m_positionTurnSemaphore;
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

