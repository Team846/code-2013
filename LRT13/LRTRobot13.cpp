#include "LRTRobot13.h"
#include "ComponentData/RobotData.h"

using namespace data;

LRTRobot13::LRTRobot13()
{
	_watchdog = wdCreate();
	
	printf("LRTRobot13 Constructed\n");
}

LRTRobot13::~LRTRobot13()
{
	printf("LRTRobot13 Destructing\n");
	
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
	
	//CANTester::Instance()->Start();

	AsyncPrinter::Printf("Creating Components\n");
	m_componentManager = new ComponentManager();
//	m_componentManager->AddComponent(new ComponentSystemUnitTest());
	m_componentManager->AddComponent(new Drivetrain());

	AsyncPrinter::Printf("Starting TeleopInputs Task\n");
	m_teleop = new TeleopInputs("TeleopInputs", 1);
	m_teleop->Start();
	
	AsyncPrinter::Printf("Starting Jaguar Tasks\n");
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->Start();
	}
	
	ConfigManager::Instance()->ConfigureAll();
}

static int TimeoutCallback(...)
{
	printf("Main loop execution time > 20ms\r\n");
	return 0;
}

void LRTRobot13::Run()
{	
	double lastUpdate = 0.0;
	
//	int e = -1;
	
	while(true)
	{
		lastUpdate = Timer::GetFPGATimestamp();
		
		wdStart(_watchdog, sysClkRateGet() / RobotConfig::LOOP_RATE,
				TimeoutCallback, 0);
		
		UpdateGameState();

		m_teleop->RunOneCycle();
		
		m_componentManager->Update();
		
		for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
		{
			(*it)->RunOneCycle();
		}
		
		wdCancel(_watchdog);
		
		double now = Timer::GetFPGATimestamp();
		double timeSpent = (now - lastUpdate) / 1000;
		
		double toSleep = (int)((1000.0 / RobotConfig::LOOP_RATE) - timeSpent) / 1000.0;

//		if(e++ % (RobotConfig::LOOP_RATE) == 0)
//			AsyncPrinter::DbgPrint("Tick: %d", e);
		
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
