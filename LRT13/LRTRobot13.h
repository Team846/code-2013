#ifndef LRT_ROBOT_13_H_
#define LRT_ROBOT_13_H_

#include "WPILib.h"

#include "wdLib.h"
#include "sysLib.h"

#include "LRTRobotBase.h"
#include "Config/ConfigManager.h"

#include "ComponentData/RobotData.h"

#include "Utils/CANTester.h"

class LRTRobot13 : public LRTRobotBase
{
	public:
		LRTRobot13();
		~LRTRobot13();
		
		void RobotInit();
	
		// Function to execute each pass in the loop; similar to a main function, but it gets run multiple times.
		void Tick();
		
	private:
		WDOG_ID _watchdog;

};

#endif
