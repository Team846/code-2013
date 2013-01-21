#include "LRTRobot13.h"
#include "ComponentData/RobotData.h"

using namespace data;

LRTRobot13::LRTRobot13()
{
	_watchdog = wdCreate();
	
	printf("LRTRobot13 constructor\n");
}

LRTRobot13::~LRTRobot13()
{
	printf("LRTRobot13 destructing\n");
	
	DELETE(m_componentManager);
}

void LRTRobot13::RobotInit()
{
	ConfigManager::Instance()->ConfigureAll();
	
	CANTester::Instance()->Start();
	
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
		
		UpdateGameState();
		
		m_componentManager->Update();
		
		wdCancel(_watchdog);
	}
}

void LRTRobot13::UpdateGameState()
{
	if (IsDisabled())
	{
		RobotData::SetRobotState(RobotData::DISABLED);
	} 
	else if (IsAutonomous())
	{
		RobotData::SetRobotState(RobotData::AUTONOMOUS);
	} 
	else
	{
		RobotData::SetRobotState(RobotData::TELEOP);
	}
}

START_ROBOT_CLASS(LRTRobot13);
