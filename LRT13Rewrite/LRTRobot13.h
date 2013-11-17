#ifndef LRT_ROBOT_13_H_
#define LRT_ROBOT_13_H_

#include <WPILib.h>

#include <wdLib.h>
#include <sysLib.h>

#include "LRTRobotBase.h"
#include "Config/ConfigManager.h"
#include "Config/RobotConfig.h"
#include "Config/DriverStationConfig.h"

#include "Utils/LCD.h"

#include "Utils/Profiler.h"



class LRTRobot13 : public LRTRobotBase
{
	public:
		LRTRobot13();
		~LRTRobot13();
		
		void RobotInit();
	
		// Function to start execution; basically a main function
		void Main();
		
	private:
		Timer timer;
		
		WDOG_ID _watchdog;

};

#endif
