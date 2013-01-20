#ifndef LRT_ROBOT_13_H_
#define LRT_ROBOT_13_H_

#include <WPILib.h>

#include <wdLib.h>
#include <sysLib.h>

#include "LRTRobotBase.h"
#include "Config/ConfigManager.h"

#include "ComponentData/RobotData.h"

#include "Components/ComponentManager.h"

#include "Utils/CANTester.h"

class LRTRobot13 : public LRTRobotBase
{
	public:
		LRTRobot13();
		~LRTRobot13();
		
		void RobotInit();
	
		// Function to start execution; basically a main function
		void Run();
		
	private:
		ComponentManager* m_componentManager;
		
		WDOG_ID _watchdog;

};

#endif
