#include "LRTRobot13.h"

LRTRobot13::LRTRobot13()
{
	_watchdog = wdCreate();
	
	printf("ctor 13");
}

LRTRobot13::~LRTRobot13()
{
	
}

void LRTRobot13::RobotInit()
{
	ConfigManager::Instance()->ConfigureAll();
	
	CANTester::Instance()->Start();
}

static int TimeoutCallback(...)
{
	return 0;
}

void LRTRobot13::Tick()
{
	wdStart(_watchdog, sysClkRateGet() / 50,
			TimeoutCallback, 0);
	
	wdCancel(_watchdog);
}

START_ROBOT_CLASS(LRTRobot13);
