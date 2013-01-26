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
	DriveEncoders::Finalize();
	CANTester::Finalize();
}

void LRTRobot13::RobotInit()
{
	AsyncPrinter::Initialize();
	
	ConfigManager::Instance()->ConfigureAll();
	
	//CANTester::Instance()->Start();
	
	m_componentManager = new ComponentManager();
	
	m_componentManager->AddComponent(new ComponentSystemUnitTest());
	
#if DANGER_CLOSE
	m_componentManager->AddComponent(new Drivetrain());
#endif
}

static int TimeoutCallback(...)
{
	return 0;
}

void LRTRobot13::Run()
{	
	double lastUpdate = 0.0;
	
	while(true)
	{
		lastUpdate = Timer::GetFPGATimestamp();
		
		static int e = -1;
		
		wdStart(_watchdog, sysClkRateGet() / RobotConfig::LOOP_RATE,
				TimeoutCallback, 0);
		
		UpdateGameState();
		
		m_componentManager->Update();
		
		wdCancel(_watchdog);
		
		double now = Timer::GetFPGATimestamp();
		double timeSpent = (now - lastUpdate) / 1000;
		
		double toSleep = (int)((1000.0 / RobotConfig::LOOP_RATE) - timeSpent) / 1000.0;

		if(e++ % (RobotConfig::LOOP_RATE * 10) == 0)
			AsyncPrinter::Println("Tick: %d", e);
		
		Wait(toSleep);
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
