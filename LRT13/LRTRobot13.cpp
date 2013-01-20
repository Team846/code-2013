#include "LRTRobot13.h"

LRTRobot13::LRTRobot13()
{
	_watchdog = wdCreate();
	
	printf("LRTRobot13 constructor\n");
}

LRTRobot13::~LRTRobot13()
{
	printf("LRTRobot13 destructing\n");
	
	DELETE(m_componentManager)
}

void LRTRobot13::RobotInit()
{
	// add basic components
	
	//ConfigManager::Instance()->ConfigureAll();
	
	//CANTester::Instance()->Start();
	
	m_componentManager = new ComponentManager();
}

static int TimeoutCallback(...)
{
	return 0;
}

void LRTRobot13::Run()
{	
	while(true)
	{
		static int e = -1;
		
		wdStart(_watchdog, sysClkRateGet() / 50,
				TimeoutCallback, 0);
		
		printf("Hello, world...i'm finally running...%d\n", ++e);
		
		m_componentManager->Update();
		
		wdCancel(_watchdog);
	}
}

START_ROBOT_CLASS(LRTRobot13);
