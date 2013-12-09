#include "DrivetrainData.h"

DrivetrainData::DrivetrainData() :
	ComponentData("DrivetrainData")
{
	ResetCommands();
}

DrivetrainData* DrivetrainData::Get()
{
	return (DrivetrainData*) ComponentData::GetComponentData("DrivetrainData");
}

void DrivetrainData::ResetCommands()
{
	m_overrideCurrentLimitForward = false;
	m_overrideCurrentLimitReverse = false;
	m_currentLimitForward = 0.5;
	m_currentLimitReverse = 0.5;
	
	memset(m_controlModes, OPEN_LOOP, sizeof(m_controlModes));
	memset(m_desiredRates, 0, sizeof(m_desiredRates));
	memset(m_positionSetpoints, 0, sizeof(m_positionSetpoints));
	memset(m_maxSpeeds, 0, sizeof(m_maxSpeeds));
	memset(m_positionSetpointChanged, 0, sizeof(m_positionSetpointChanged));
	memset(m_positionStart, 0, sizeof(m_positionStart));
}

void DrivetrainData::Log()
{
	LogToFile(&m_controlModes, "ControlModes");
	LogToFile(&m_desiredOpenLoopOutputs, "OpenLoopOutputs");
	LogToFile(&m_desiredRates, "DesiredRates");
	LogToFile(&m_positionSetpoints, "PositionSetpoints");
	LogToFile(&m_maxSpeeds, "MaxSpeeds");
	LogToFile(&m_positionStart, "PositionStartLocations");
	LogToFile(&m_zeroHeading, "ZeroHeading");
	LogToFile(&m_positionSetpointChanged, "PositionSetpointChanged");
	LogToFile(&m_syncArc, "SyncArc");
	LogToFile(&m_overrideCurrentLimitForward, "OverrideForwardCurrentLimit");
	LogToFile(&m_overrideCurrentLimitReverse, "OverrideReverseCurrentLimit");
	LogToFile(&m_currentLimitForward, "ForwardCurrentLimit");
	LogToFile(&m_currentLimitReverse, "ReverseCurrentLimit");
}

void DrivetrainData::SetControlMode(DrivetrainData::Axis axis, DrivetrainData::ControlMode mode)
{
	m_controlModes[axis] = mode;
}

void DrivetrainData::SetVelocitySetpoint(DrivetrainData::Axis axis, float velocity)
{
	m_desiredRates[axis] = velocity;
}
