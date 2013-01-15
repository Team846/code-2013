#include "DrivetrainData.h"

#warning Needs implementation.  Dummy methods are so that we can run the code.

using namespace data;
using namespace data::drivetrain;

ControlMode DrivetrainData::getControlMode(ForwardOrTurn mode)
{
	
}

void DrivetrainData::setOpenLoopOutput(ForwardOrTurn mode, double setpoint) { }
void DrivetrainData::setVelocitySetpoint(ForwardOrTurn mode, double setpoint) { }
void DrivetrainData::setRelativePositionSetpoint(ForwardOrTurn mode, double setpoint,
		double maxspeed) { }

SEM_ID DrivetrainData::positionOperationSemaphore(ForwardOrTurn mode, double errorThreshold) { }
bool DrivetrainData::isDesiredPositionOperationComplete(ForwardOrTurn mode,
		double errorThreshold) { return false; }

double DrivetrainData::getOpenLoopOutput(ForwardOrTurn mode) { return 0.0; }
double DrivetrainData::getVelocitySetpoint(ForwardOrTurn mode) { return 0.0; }
double DrivetrainData::getRelativePositionSetpoint(ForwardOrTurn mode) { return 0.0; }
double DrivetrainData::getPositionControlMaxSpeed(ForwardOrTurn mode) { return 0.0; }
