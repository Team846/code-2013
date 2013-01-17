#include "LRTRobot13.h"

LRTRobot13::LRTRobot13()
{
	_watchdog = wdCreate();
	
	printf("LRTRobot13 constructor\n");
}

LRTRobot13::~LRTRobot13()
{
	printf("LRTRobot13 destructing\n");
}

void LRTRobot13::RobotInit()
{
	//ConfigManager::Instance()->ConfigureAll();
	
	//CANTester::Instance()->Start();
}

static int TimeoutCallback(...)
{
	return 0;
}

void LRTRobot13::Tick()
{
	static int e = -1;
	
	wdStart(_watchdog, sysClkRateGet() / 50,
			TimeoutCallback, 0);
	
	printf("Hello, world...i'm finally running...%d\n", ++e);
	
	wdCancel(_watchdog);
}

START_ROBOT_CLASS(LRTRobot13);
