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
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->Abort();
	}
	for (vector<Pneumatics*>::iterator it = Pneumatics::pneumatic_vector.begin(); it < Pneumatics::pneumatic_vector.end(); it++)
	{
		(*it)->Abort();
	}
	LogManager::Instance()->Abort();
	
	DELETE(m_componentManager);
	DELETE(m_leds);
	SmarterDashboard::Close();
	
	// Finalize all singletons
	ComponentData::Finalize();
	AsyncPrinter::Finalize();
	ConfigManager::Finalize();
	LogManager::Finalize();
	LCD::Finalize();
	DriveEncoders::Finalize();
	AutoActions::Finalize();
	IMU::Finalize();
	Pneumatics::DestroyCompressor();
}


void LRTRobot13::RobotInit()
{
	timer.Start();
	AsyncPrinter::Println("Creating Components...");
	m_componentManager = new ComponentManager();
	m_componentManager->CreateComponents();
	
	AsyncPrinter::Println("Starting TeleopInputs Task...");
	m_teleop = new TeleopInputs("TeleopInputs");
	
	AsyncPrinter::Println("initializing AutonomousRoutines...");
	m_auton = new AutonomousRoutines();
	
	AsyncPrinter::Println("Starting Jaguar Tasks...");
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->Start();
	}
	AsyncPrinter::Println("Starting Pneumatics Tasks...");
	for (vector<Pneumatics*>::iterator it = Pneumatics::pneumatic_vector.begin(); it < Pneumatics::pneumatic_vector.end(); it++)
	{
		(*it)->Start();
	}
		
	Pneumatics::CreateCompressor();
	
	AsyncPrinter::Println("Starting LogManager Task...");
	LogManager::Instance()->Start();

	AsyncPrinter::Println("Starting LCD Task...");
	LCD::Instance()->Start();
	
	AsyncPrinter::Println("Configuring...");
	ConfigManager::Instance()->ConfigureAll();
	
	AsyncPrinter::Println("Starting SmarterDashboard service...");
	SmarterDashboard::Start();
	AsyncPrinter::Println("Starting AutoAim...");
	m_autoaim = new AutoAim();
	m_autoaim->Start();
	
	//AsyncPrinter::Println("Creating the IMU...");
	//IMU::Instance()->Start();
	
	AsyncPrinter::Println("Starting LED Task");
	m_leds = new LEDIndicators();
	m_leds->Start();

	
	AsyncPrinter::Println("Reticulating splines...");
}

int looptime;
static int TimeoutCallback(...)
{
	printf("Main loop execution time > 20ms took %f\r\n", ((GetFPGATime() - looptime) / 1E3));
	
	AsyncPrinter::Printf("======================================\n");
	AsyncPrinter::Printf("PROFILED TIMES (over 20ms): \n");
	AsyncPrinter::Printf("\n");
	map<string, double>* times = Profiler::GetLastTimes();
	
	for(map<string, double>::iterator it = times->begin(); it != times->end(); ++it)
	{
		string name = it->first;
		double time = it->second;
		
		AsyncPrinter::Printf("%s: %f ms\n", name.c_str(), time * 1000.0);
	}
	AsyncPrinter::Printf("======================================\n");
	
	return 0;
}

void LRTRobot13::Tick()
{
	Profiler::BeginActivity("Tick");
	
	
	looptime = GetFPGATime();
	wdStart(_watchdog, sysClkRateGet() / RobotConfig::LOOP_RATE,
			TimeoutCallback, 0);
	
	LCD::Instance()->UpdateGameTime(timer.Get());
	// Update current game state
	UpdateGameState();

	// Update appropriate operation controllers
	if (RobotData::GetCurrentState() == RobotData::AUTONOMOUS )
	{
		m_auton->Update();
	}
	else if (RobotData::GetCurrentState() == RobotData::TELEOP)
	{
		m_teleop->Tick();
		m_auton->TeleopTick(); // Handles the automation involving vision
//		m_teleop->RunOneCycle();
//		AsyncPrinter::Printf("TELEOP!\n");
	}
	else // Disabled
	{
//		AsyncPrinter::Printf("Disabled!\n");
		m_teleop->Tick();
//		m_teleop->RunOneCycle();
	}
	
	// Update all components
	m_componentManager->Update();
	
	// Update all jaguars
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->RunOneCycle();
		if (!(*it)->StatusOK())
		{	
			AsyncPrinter::Printf("[Error] Jaguar %d (%s): ", (*it)->GetChannel(), (*it)->GetName());
			for (int i = 16; i < 0; i--)
			{
				AsyncPrinter::Printf("%d", ((*it)->GetFaults() & ( 1 << i )) >> i);
			}
			AsyncPrinter::Printf("\n");
		}
	}
	// Update all pneumatics
	for (vector<Pneumatics*>::iterator it = Pneumatics::pneumatic_vector.begin(); it < Pneumatics::pneumatic_vector.end(); it++)
	{
		(*it)->RunOneCycle();
	}
		
	// Update all talons
	for (vector<LRTTalon*>::iterator it = LRTTalon::talon_vector.begin(); it < LRTTalon::talon_vector.end(); it++)
	{
		(*it)->Update();
	}

	// Update pneumatics
	if (DriverStation::GetInstance()->GetDigitalIn(DriverStationConfig::DigitalIns::COMPRESSOR))// && RobotData::GetCurrentState() != RobotData::AUTONOMOUS)
	{
		Pneumatics::SetCompressor(true);
	}
	else
	{
		Pneumatics::SetCompressor(false);
	}
	
	// Update LogManager
	LogManager::Instance()->RunOneCycle();
	
	if(DriverStation::GetInstance()->GetBatteryVoltage() < 7.5)
		AsyncPrinter::Printf("[WARNING] Battery voltage: %f\n", DriverStation::GetInstance()->GetBatteryVoltage());

	LCD::Instance()->RunOneCycle();
	
	//Update LEDs
	m_leds->RunOneCycle();
	
	//Update AutoAim
	m_autoaim->RunOneCycle();
	
	// Update SmarterDashboard -- this should be the last thing!
	SmarterDashboard::Instance()->Tick();
	
//	static int tick = 0;
//	
//	if(tick++ % 50 == 0)
//	{
//		AsyncPrinter::Printf("======================================\n");
//		AsyncPrinter::Printf("PROFILED TIMES: \n");
//		AsyncPrinter::Printf("\n");
//		map<string, double>* times = Profiler::GetLastTimes();
//		
//		for(map<string, double>::iterator it = times->begin(); it != times->end(); ++it)
//		{
//			string name = it->first;
//			double time = it->second;
//			
//			AsyncPrinter::Printf("%s: %f\n", name.c_str(), time);
//		}
//		AsyncPrinter::Printf("======================================\n");
//	}
	
	wdCancel(_watchdog);
	Profiler::End("Tick");
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
