#ifndef DRIVETRAIN_INPUTS_H_
#define DRIVETRAIN_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/DrivetrainData.h"

/*!
 * @brief Process joystick information for controlling the drivetrain.
 */
class DrivetrainInputs : public InputProcessor
{
public:
	DrivetrainInputs();
	
	void Update();
	
private:
	DebouncedJoystick *m_driver_stick;
	DebouncedJoystick *m_driver_wheel;
	
	DrivetrainData *drivetrainData;

	bool lastStop;
	double stoppedForward;
	double stoppedTurn;
	int driveSign;
};

#endif
