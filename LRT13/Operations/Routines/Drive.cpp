#include "Drive.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>

Drive::Drive(double distance, double maxSpeed, double errorThreshold)
{
	m_distance = distance;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
}

void Drive::Run()
{
	m_componentData->drivetrainData->setControlMode(data::drivetrain::FORWARD, data::drivetrain::POSITION_CONTROL);
	m_componentData->drivetrainData->setControlMode(data::drivetrain::TURN, data::drivetrain::VELOCITY_CONTROL);
	m_componentData->drivetrainData->setRelativePositionSetpoint(data::drivetrain::FORWARD, m_distance, m_maxSpeed);
	m_componentData->drivetrainData->setVelocitySetpoint(data::drivetrain::TURN, 0.0);
}

bool Drive::Completed()
{
	return fabs(m_componentData->drivetrainData->getRelativePositionSetpoint(data::drivetrain::FORWARD)) < m_errorThreshold;
}
