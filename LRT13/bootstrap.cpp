#include "LRTRobot13.h"
#include "UnitTests/UnitTest.h"
#include "UnitTests/ShooterTest.h"
#include "UnitTests/EncodersUnitTest.h"
#include "UnitTests/GearToothTest.h"
#include "UnitTests/InputUnitTest.h"
#include "UnitTests/ServoTest.h"
#include "UnitTests/ProximityTest.h"
#include "UnitTests/NetUnitTest.h"
#include "UnitTests/NetServerUnitTest.h"

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
//	return new ProximityTest();
//	return new InputUnitTest();
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
