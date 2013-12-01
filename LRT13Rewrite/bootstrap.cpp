#include "LRTRobot13.h"

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
