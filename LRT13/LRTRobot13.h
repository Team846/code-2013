#ifndef LRT_ROBOT_13_H_
#define LRT_ROBOT_13_H_

#include <WPILib.h>

#include <wdLib.h>
#include <sysLib.h>

#include "LRTRobotBase.h"
#include "Config/ConfigManager.h"
#include "Config/RobotConfig.h"
#include "Config/DriverStationConfig.h"

#include "ComponentData/RobotData.h"

#include "Components/ComponentManager.h"
#include "Components/ComponentSystemUnitTest.h"
#include "Components/Drivetrain.h"

#include "Log/LogManager.h"

#include "Operations/AutonomousRoutines.h"
#include "Operations/TeleopInputs.h"
#include "SpeedController/AsyncCANJaguar.h"
#include "SpeedController/LRTTalon.h"

#include "Utils/LCD.h"

#include "Pneumatics/Pneumatics.h"

#include "Sensors/DriveEncoders.h"
#include "Sensors/IMU.h"

#include "Utils/SmarterDashboard.h"
#include "Sensors/AutoAim.h"
#include "Operations/AutoActions.h"

#include "Components/LEDIndicators.h"

#include "Utils/Profiler.h"



class LRTRobot13 : public LRTRobotBase
{
	public:
		LRTRobot13();
		~LRTRobot13();
		
		void RobotInit();
	
		// Function to start execution; basically a main function
		void Tick();
		
	private:
		void UpdateGameState();
		
		ComponentManager* m_componentManager;
		TeleopInputs* m_teleop;
		AutonomousRoutines* m_auton;
		AutoAim* m_autoaim;
		
		LEDIndicators* m_leds;
		
		WDOG_ID _watchdog;

};

#endif
