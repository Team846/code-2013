#ifndef DRIVETRAIN_DATA_H
#define DRIVETRAIN_DATA_H

#include "ComponentData.h"
#include <semLib.h>


class DriveEncoders;

namespace data
{
/*!
 * @brief Namespace containing data pertaining to the drivetrain.
 * @author Tony Peng, Brian Axelrod, Raphael Chang
 */
namespace drivetrain
{
	enum ControlMode
	{
		POSITION_CONTROL, VELOCITY_CONTROL, OPEN_LOOP
	};

	enum ForwardOrTurn
	{
		FORWARD = 0, TURN = 1
	};

	enum Side
	{
		LEFT = 2, RIGHT = 3
	};

	class DrivetrainData
	{
	public:
		DrivetrainData();
		~DrivetrainData();
		
		ControlMode getControlMode(ForwardOrTurn axis);
	
		void setOpenLoopOutput(ForwardOrTurn axis, double setpoint);
		void setVelocitySetpoint(ForwardOrTurn axis, double setpoint);
		
		void setRelativePositionSetpoint(ForwardOrTurn axis, double setpoint,
				double maxSpeed);
		void setMaxPositionControlSpeed(ForwardOrTurn axis, double maxSpeed);
		
		void setControlMode(ForwardOrTurn axis, ControlMode control);
	

		SEM_ID positionOperationSemaphore(ForwardOrTurn axis, double errorThreshold);
		
		bool isDesiredPositionOperationComplete(ForwardOrTurn axis,
				double errorThreshold);
	
		double getOpenLoopOutput(ForwardOrTurn axis);
		double getVelocitySetpoint(ForwardOrTurn axis);
		double getRelativePositionSetpoint(ForwardOrTurn axis);
		double getPositionControlMaxSpeed(ForwardOrTurn axis);
	
	private:
		double getCurrentPos(ForwardOrTurn axis);
		
		DriveEncoders *m_driveEncoders;
		ControlMode m_controlModes[2];
		double m_desiredOpenLoopOutputs[2];
		double m_desiredRates[2];
		double m_positionSetpoints[2];
		double m_maxSpeeds[2];
		SEM_ID m_positionFwdSemaphore;
		SEM_ID m_positionTurnSemaphore;
	};
}
}

#endif
