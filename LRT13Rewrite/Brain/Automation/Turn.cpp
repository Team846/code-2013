#include "Turn.h"
#include "../../ComponentData/DrivetrainData.h"
#include <math.h>

Turn::Turn(double angle, double maxSpeed, double errorThreshold) :
	Automation("Turn")
{
	m_angle = angle;
	m_maxSpeed = maxSpeed;
	m_errorThreshold = errorThreshold;
	m_drivetrain = DrivetrainData::Get();
}

void Turn::AllocateResources()
{
	AllocateResource(TURN);
}

bool Turn::Start()
{
	m_drivetrain->SetControlMode(DrivetrainData::TURN, DrivetrainData::POSITION_CONTROL);
	//m_drivetrain->SetRelativePositionSetpoint(DrivetrainData::TURN, m_angle, m_maxSpeed, true);
	return true;
}

bool Turn::Run()
{
	return true;//fabs(m_drivetrain->GetRelativePositionSetpoint(DrivetrainData::TURN)) < m_errorThreshold;
}

bool Turn::Abort()
{
	return true;
}
