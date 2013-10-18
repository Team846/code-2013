#include "Drive.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>
#include "../../Utils/Util.h"

Drive::Drive(double distance, double maxSpeed, double errorThreshold, bool continuous)
{
	m_distance = distance;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
	m_continuous = continuous;
}

void Drive::Run()
{
	if (!m_continuous)
	{
		m_componentData->drivetrainData->setControlMode(data::drivetrain::FORWARD, data::drivetrain::POSITION_CONTROL);
	}
	else
	{
		m_componentData->drivetrainData->setControlMode(data::drivetrain::FORWARD, data::drivetrain::VELOCITY_CONTROL);
		m_componentData->drivetrainData->setVelocitySetpoint(data::drivetrain::FORWARD, Util::Sign(m_distance) * m_maxSpeed);
	}
	m_componentData->drivetrainData->setRelativePositionSetpoint(data::drivetrain::FORWARD, m_distance, m_maxSpeed, true);
}

bool Drive::Completed()
{
	return fabs(m_componentData->drivetrainData->getRelativePositionSetpoint(data::drivetrain::FORWARD)) < m_errorThreshold;
}

void Drive::Stop()
{
	if (m_continuous)
	{
		m_componentData->drivetrainData->setControlMode(data::drivetrain::FORWARD, data::drivetrain::POSITION_CONTROL);
	}
}
