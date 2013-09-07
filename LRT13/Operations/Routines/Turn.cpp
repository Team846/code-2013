#include "Turn.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>

Turn::Turn(double angle, double maxSpeed, double errorThreshold)
{
	m_angle = angle;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
}

void Turn::Run()
{
	m_componentData->drivetrainData->setControlMode(data::drivetrain::TURN, data::drivetrain::POSITION_CONTROL);
	m_componentData->drivetrainData->setControlMode(data::drivetrain::FORWARD, data::drivetrain::VELOCITY_CONTROL);
	m_componentData->drivetrainData->setRelativePositionSetpoint(data::drivetrain::TURN, m_angle, m_maxSpeed);
	m_componentData->drivetrainData->setVelocitySetpoint(data::drivetrain::FORWARD, 0.0);
}

bool Turn::Completed()
{
	return fabs(m_componentData->drivetrainData->getRelativePositionSetpoint(data::drivetrain::TURN)) < m_errorThreshold;
}
