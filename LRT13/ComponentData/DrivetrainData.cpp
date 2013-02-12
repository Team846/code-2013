#include "DrivetrainData.h"

using namespace data;
using namespace data::drivetrain;

DrivetrainData::DrivetrainData()
{
	// these arrays are statically allocated and thus cannot be deallocated... -tp
	memset(m_controlModes, OPEN_LOOP, sizeof(m_controlModes));
	memset(m_desiredRates, 0, sizeof(m_desiredRates));
	memset(m_desiredPositions, 0, sizeof(m_desiredPositions));
	memset(m_maxSpeeds, 0, sizeof(m_maxSpeeds));
	memset(m_status.output, 0, sizeof(m_status.output));
	memset(m_status.position, 0, sizeof(m_status.position));
	memset(m_status.velocity, 0, sizeof(m_status.velocity));
	memset(m_lastPosition, 0, sizeof(m_lastPosition));
	m_positionFwdSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	m_positionTurnSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
}

DrivetrainData::~DrivetrainData()
{
	semDelete(m_positionFwdSemaphore);
	semDelete(m_positionTurnSemaphore);
}

ControlMode DrivetrainData::getControlMode(ForwardOrTurn mode)
{
	return m_controlModes[mode];
}

void DrivetrainData::setOpenLoopOutput(ForwardOrTurn mode, double setpoint)
{
}

void DrivetrainData::setVelocitySetpoint(ForwardOrTurn mode, double setpoint)
{
	m_desiredRates[mode] = setpoint;
}

void DrivetrainData::setRelativePositionSetpoint(ForwardOrTurn mode,
		double setpoint, double maxspeed)
{
	semTake(mode == drivetrain::FORWARD ? m_positionFwdSemaphore : m_positionTurnSemaphore, NO_WAIT); // If this cannot take a semaphore another operation is still in progress. Ignore and update the desired position.
	
	m_lastPosition[mode] = m_status.position[mode];
	m_desiredPositions[mode] = setpoint;
	m_maxSpeeds[mode] = maxspeed;
}

void DrivetrainData::setControlMode(ForwardOrTurn mode, ControlMode control)
{
	m_controlModes[mode] = control;
}

void DrivetrainData::updatePositions(double forward, double turn)
{
	m_status.position[FORWARD] = forward;
	m_status.position[TURN] = turn;
}
void DrivetrainData::updateVelocities(double forward, double turn)
{
	m_status.velocity[FORWARD] = forward;
	m_status.velocity[TURN] = turn;
}

SEM_ID DrivetrainData::positionOperationSemaphore(ForwardOrTurn mode,
		double errorThreshold)
{
	return mode == drivetrain::FORWARD ? m_positionFwdSemaphore : m_positionTurnSemaphore;
}

bool DrivetrainData::isDesiredPositionOperationComplete(ForwardOrTurn mode,
		double errorThreshold)
{
	return false;
}

double DrivetrainData::getOpenLoopOutput(ForwardOrTurn mode)
{
	return 0.0;
}
double DrivetrainData::getVelocitySetpoint(ForwardOrTurn mode)
{
	return m_desiredRates[mode];
}
double DrivetrainData::getRelativePositionSetpoint(ForwardOrTurn mode)
{
	return m_desiredPositions[mode];
}
double DrivetrainData::getPositionControlMaxSpeed(ForwardOrTurn mode)
{
	return m_maxSpeeds[mode];
}

double DrivetrainData::getPositionControlStartingPosition(ForwardOrTurn mode)
{
	return m_lastPosition[mode];
}
