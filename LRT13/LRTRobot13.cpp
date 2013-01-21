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
	
	// finalize all singletons
	ComponentData::Finalize();
	AsyncPrinter::Finalize();
	ConfigManager::Finalize();
	LogManager::Finalize();
	CANTester::Finalize();
}

void LRTRobot13::RobotInit()
{
	ConfigManager::Instance()->ConfigureAll();
	
	CANTester::Instance()->Start();
	
	m_componentManager = new ComponentManager();
	
	m_componentManager->AddComponent(new ComponentSystemUnitTest());
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
		
		UpdateGameState();
		
		m_componentManager->Update();
		
		printf("Hello, world...i'm finally running...%d\n", ++e);
		
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
