#include "LRTRobot13.h"
#include "Utils/UnitTest.h"
#include "Components/ShooterTest.h"

/* ----- BOOTSTRAPPER ----- */
/* This file is very important.
 * If you don't have this, you don't have any
 * loaded code. :)
 */


/*!
 * Pre-defined macro is:
 * START_ROBOT_CLASS(class);
 * 
 * The reason for expanding to macro is to make it easier to perform unit tests.
 */
RobotBase* FRC_userClassFactory()
{
	return new LRTRobot13();
}

extern "C"
{
	INT32 FRC_UserProgram_StartupLibraryInit()
	{
		RobotBase::startRobotTask((FUNCPTR) FRC_userClassFactory);
		return 0;
	}
}
