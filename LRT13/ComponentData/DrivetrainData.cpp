#include "DrivetrainData.h"

#warning Needs implementation.  Dummy methods are so that we can run the code.

using namespace data;
using namespace data::drivetrain;

DrivetrainData::DrivetrainData()
{
	memset(m_controlModes, OPEN_LOOP, sizeof(m_controlModes));
	memset(m_desiredRates, 0, sizeof(m_desiredRates));
	memset(m_desiredPositions, 0, sizeof(m_desiredPositions));
	memset(m_maxSpeeds, 0, sizeof(m_maxSpeeds));
	m_positionFwdSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	m_positionTurnSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
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
	if (setpoint != m_desiredPositions[mode])
	{
		semTake(m_positionFwdSemaphore, WAIT_FOREVER);
	}
	
	m_desiredPositions[mode] = setpoint;
	m_maxSpeeds[mode] = maxspeed;
}

void DrivetrainData::setControlMode(ForwardOrTurn mode, ControlMode control)
{
	m_controlModes[mode] = control;
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
