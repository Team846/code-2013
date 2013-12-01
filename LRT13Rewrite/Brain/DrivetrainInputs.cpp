#include "DrivetrainInputs.h"

DrivetrainInputs::DrivetrainInputs()
{
	m_driver_stick = LRTDriverStation::Instance()->GetDriverStick();
	m_driver_wheel = LRTDriverStation::Instance()->GetDriverWheel();
	
//	RegisterResource(drivetrainData);
}

void DrivetrainInputs::Update()
{
	
}
