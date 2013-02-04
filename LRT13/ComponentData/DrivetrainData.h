#ifndef DRIVETRAIN_DATA_H
#define DRIVETRAIN_DATA_H

#include "ComponentData.h"
#include <semLib.h>

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

	struct DrivetrainStatus
	{
		double velocity[4];
		double position[4];
		double output[4];
	};

	class DrivetrainData
	{
	public:
		DrivetrainData();
		
		ControlMode getControlMode(ForwardOrTurn mode);
	
		void setOpenLoopOutput(ForwardOrTurn mode, double setpoint);
		void setVelocitySetpoint(ForwardOrTurn mode, double setpoint);
		void setRelativePositionSetpoint(ForwardOrTurn mode, double setpoint,
				double maxspeed);
		void setControlMode(ForwardOrTurn mode, ControlMode control);
	
		SEM_ID positionOperationSemaphore(ForwardOrTurn mode, double errorThreshold);
		bool isDesiredPositionOperationComplete(ForwardOrTurn mode,
				double errorThreshold);
	
		double getOpenLoopOutput(ForwardOrTurn mode);
		double getVelocitySetpoint(ForwardOrTurn mode);
		double getRelativePositionSetpoint(ForwardOrTurn mode);
		double getPositionControlMaxSpeed(ForwardOrTurn mode);
	
	private:
		ControlMode m_controlModes[2];
		double m_desiredRates[2];
		double m_desiredPositions[2];
		double m_maxSpeeds[2];
		SEM_ID m_positionSemaphore;
	};
}
}

#endif
