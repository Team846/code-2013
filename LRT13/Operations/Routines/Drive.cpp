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
	m_componentData->drivetrainData->setRelativePositionSetpoint(data::drivetrain::FORWARD, m_distance, m_maxSpeed);
}

bool Drive::Completed()
{
	return fabs(m_componentData->drivetrainData->getRelativePositionSetpoint(data::drivetrain::FORWARD)) < m_errorThreshold;
}
