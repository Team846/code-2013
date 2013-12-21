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
	
}

Automation::Status Turn::Start(Event *trigger)
{
	m_drivetrain->SetControlMode(DrivetrainData::TURN, DrivetrainData::POSITION_CONTROL);
	//m_drivetrain->SetRelativePositionSetpoint(DrivetrainData::TURN, m_angle, m_maxSpeed, true);
	return SUCCESS;
}

bool Turn::Run()
{
	return true;//fabs(m_drivetrain->GetRelativePositionSetpoint(DrivetrainData::TURN)) < m_errorThreshold;
}

Automation::Status Turn::Abort(Event *trigger)
{
	return SUCCESS;
}
