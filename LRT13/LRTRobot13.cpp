#include "LRTRobot13.h"
#include "ComponentData/RobotData.h"
#include "Components/ConfigLoader.h"
using namespace data;

LRTRobot13::LRTRobot13()
{
	_watchdog = wdCreate();
	
	printf("LRTRobot13 Constructed\n");
}

LRTRobot13::~LRTRobot13()
{
	printf("LRTRobot13 Destructing\n");
	
	// Stop all tasks
	m_auton->Abort(0, 1.0);
//	m_teleop->Abort();
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->Abort();
	}
	Pneumatics::Instance()->Abort();
	LogManager::Instance()->Abort();
	
	DELETE(m_componentManager);
	
	// Finalize all singletons
	ComponentData::Finalize();
	AsyncPrinter::Finalize();
	ConfigManager::Finalize();
	LogManager::Finalize();
	Pneumatics::Finalize();
	DriveEncoders::Finalize();
}

void LRTRobot13::RobotInit()
{
	AsyncPrinter::Println("Creating Components");
	m_componentManager = new ComponentManager();
	m_componentManager->CreateComponents();

	AsyncPrinter::Println("Starting TeleopInputs Task");
	m_teleop = new TeleopInputs("TeleopInputs");
//	m_teleop->Start();
	
	AsyncPrinter::Println("initializing AutonomousRoutines");
	m_auton = new AutonomousRoutines();
	
	AsyncPrinter::Println("Starting Jaguar Tasks");
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->Start();
	}
	
	AsyncPrinter::Println("Starting Pneumatics Task");
	Pneumatics::Instance()->Start();
	
	AsyncPrinter::Println("Starting LogManager Task");
	LogManager::Instance()->Start();
	
	ConfigManager::Instance()->ConfigureAll();
}

static int TimeoutCallback(...)
{
	printf("Main loop execution time > 20ms\r\n");
	return 0;
}

void LRTRobot13::Tick()
{
	wdStart(_watchdog, sysClkRateGet() / RobotConfig::LOOP_RATE,
			TimeoutCallback, 0);
	
	// Update current game state
	UpdateGameState();

	// Update appropriate operation controllers
	if (RobotData::GetCurrentState() == RobotData::AUTONOMOUS )
	{
		if (RobotData::GetLastState() != RobotData::GetCurrentState())
			m_auton->Start();
	}
	else if (RobotData::GetCurrentState() == RobotData::TELEOP)
	{
		m_auton->TeleopTick(); // Handles the automation involving vision
		if (m_auton->IsRunning())
		{
			m_auton->Abort(0, 1.0);
			AsyncPrinter::Printf("Killing auton\n");
		}
//		m_teleop->RunOneCycle();
//		AsyncPrinter::Printf("TELEOP!\n");
		m_teleop->Tick();
	}
	else // Disabled
	{
//		AsyncPrinter::Printf("Disabled!\n");
		m_teleop->Tick();
//		m_teleop->RunOneCycle();
		if (m_auton->IsRunning())
		{
			m_auton->Abort(1.0, 1.0);
		}
	}
	
	// Update all components
	m_componentManager->Update();
	
	// Update all jaguars
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->RunOneCycle();
	}

	// Update pneumatics
	if (DriverStation::GetInstance()->GetDigitalIn(DriverStationConfig::DigitalIns::PNEUMATICS))
	{
		Pneumatics::Instance()->setCompressor(true);
	}
	else
	{
		Pneumatics::Instance()->setCompressor(false);
	}
	Pneumatics::Instance()->RunOneCycle();
	
	// Update LogManager
	LogManager::Instance()->RunOneCycle();
	
	wdCancel(_watchdog);
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
