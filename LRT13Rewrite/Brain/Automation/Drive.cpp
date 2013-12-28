#include "Drive.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>
#include "../../Utils/Util.h"

Drive::Drive(double distance, double maxSpeed, double errorThreshold, bool continuous) :
	Automation("Drive")
{
	m_distance = distance;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
	m_continuous = continuous;
	m_drivetrain = DrivetrainData::Get();
}

void Drive::AllocateResources()
{
	AllocateResource(DRIVE);
}

bool Drive::Start()
{
	if (!m_continuous)
	{
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::POSITION_CONTROL);
	}
	else
	{
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::VELOCITY_CONTROL);
		m_drivetrain->SetVelocitySetpoint(DrivetrainData::FORWARD, Util::Sign(m_distance) * m_maxSpeed);
	}
	//m_drivetrain->SetRelativePositionSetpoint(DrivetrainData::FORWARD, m_distance, m_maxSpeed, true);
	return true;
}

bool Drive::Run()
{
	return true;//fabs(m_drivetrain->GetRelativePositionSetpoint(DrivetrainData::FORWARD)) < m_errorThreshold;
}

bool Drive::Abort()
{
	if (m_continuous)
	{
		m_drivetrain->SetControlMode(DrivetrainData::FORWARD, DrivetrainData::POSITION_CONTROL);
	}
	return true;
}
