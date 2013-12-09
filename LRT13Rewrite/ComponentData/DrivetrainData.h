#ifndef DRIVETRAIN_DATA_H_
#define DRIVETRAIN_DATA_H_

#include "ComponentData.h"

class DrivetrainData : public ComponentData
{
public:
	enum ControlMode
	{
		POSITION_CONTROL, VELOCITY_CONTROL, OPEN_LOOP
	};

	enum Axis
	{
		FORWARD = 0, TURN = 1
	};

	enum Side
	{
		LEFT = 0, RIGHT = 1
	};
	
	DrivetrainData();
	static DrivetrainData* Get();
	void ResetCommands();
	void Log();
	
	void SetControlMode(DrivetrainData::Axis axis, DrivetrainData::ControlMode mode);
	void SetVelocitySetpoint(DrivetrainData::Axis axis, float velocity);
	
private:
	ControlMode m_controlModes[2];
	double m_desiredOpenLoopOutputs[2];
	double m_desiredRates[2];
	double m_positionSetpoints[2];
	double m_maxSpeeds[2];
	double m_positionStart[2];
	float m_zeroHeading;
	bool m_positionSetpointChanged[2];
	bool m_syncArc;
	bool m_overrideCurrentLimitForward;
	bool m_overrideCurrentLimitReverse;
	float m_currentLimitForward;
	float m_currentLimitReverse;
};

#endif
