#include "Arc.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>
#include "../../Utils/Util.h"

Arc::Arc(double distance, double angle, double maxSpeed, double errorThreshold, bool continuous)
{
	m_distance = distance;
	m_angle = angle;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
	m_continuous = continuous;
}

void Arc::Run()
{
	// Arc length = angle * distance * PI / (180 * sin(angle))
	// Resulting angle = 2 * angle
	// Central angle = 2 * angle
	// Radius = Chord Length / (2 * sin(Angle))
	if (!m_continuous)
	{
		m_componentData->drivetrainData->setControlMode(data::drivetrain::FORWARD, data::drivetrain::POSITION_CONTROL);
	}
	else
	{
		m_componentData->drivetrainData->setControlMode(data::drivetrain::FORWARD, data::drivetrain::VELOCITY_CONTROL);
		m_componentData->drivetrainData->setVelocitySetpoint(data::drivetrain::FORWARD, Util::Sign(m_distance) * m_maxSpeed);
	}
	m_componentData->drivetrainData->setRelativePositionSetpoint(data::drivetrain::FORWARD, m_angle * m_distance * acos(-1) / (180 * (sin(m_angle * acos(-1) / 180))), m_maxSpeed, true);
	m_componentData->drivetrainData->setControlMode(data::drivetrain::TURN, data::drivetrain::POSITION_CONTROL);
	m_componentData->drivetrainData->setRelativePositionSetpoint(data::drivetrain::TURN, 2 * m_angle, m_maxSpeed, true);
	m_componentData->drivetrainData->syncArc();
}

bool Arc::Completed()
{
	return fabs(m_componentData->drivetrainData->getRelativePositionSetpoint(data::drivetrain::FORWARD)) < m_errorThreshold && fabs(m_componentData->drivetrainData->getRelativePositionSetpoint(data::drivetrain::TURN)) < m_errorThreshold;
}

void Arc::Stop()
{
	if (m_continuous)
	{
		m_componentData->drivetrainData->setControlMode(data::drivetrain::FORWARD, data::drivetrain::POSITION_CONTROL);
	}
}
