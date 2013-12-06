#include "LRTRobot13.h"

#include "Brain/Brain.h"
#include "Components/Component.h"
#include "Actuators/Actuator.h"
#include "Actuators/AsyncCANJaguar.h"
#include "Actuators/Pneumatics.h"
#include "RobotState.h"
#include "DriverStation/LRTDriverStation.h"

#include "Config/ConfigRuntime.h"
#include "Config/ConfigPortMappings.h"
#include "Config/RobotConfig.h"
#include "Config/DriverStationConfig.h"
#include "Utils/LCD.h"
#include "Logging/Logger.h"

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
	
	Component::DestroyComponents();
	ConfigPortMappings::Finalize();
	ConfigRuntime::Finalize();
	Logger::Finalize();
	Pneumatics::DestroyCompressor();
	Brain::Finalize();
	LCD::Finalize();
	AsyncPrinter::Finalize();
	LRTDriverStation::Finalize();
	RobotState::Finalize();
}

void LRTRobot13::RobotInit()
{
	// Initialize global robot state object
	RobotState::Initialize();
	
	// Initialize Utilities
	AsyncPrinter::Initialize();
	LCD::Instance()->Start();

	// Create the Driver Station
	AsyncPrinter::Println("Initializing Driver Station...");
	LRTDriverStation::Initialize();
	
	// Read port mappings
	AsyncPrinter::Println("Loading Port Mappings...");
	ConfigPortMappings::Instance()->Load();
	
	// Create all components
	AsyncPrinter::Println("Creating Components...");
	Component::CreateComponents();
	
	// Initialize the Brain
	AsyncPrinter::Println("Initializing Brain...");
	Brain::Initialize();
	
	// Start Actuator tasks
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

	// Create and start compressor
	AsyncPrinter::Println("Creating Pneumatics Compressor...");
	Pneumatics::CreateCompressor();

	// Initialize the Logger
	AsyncPrinter::Println("Initializing Logger...");
	Logger::Instance()->Initialize();
	
	// Apply runtime configuration
	ConfigRuntime::ConfigureAll();
}

static int TimeoutCallback(...)
{
	printf("Main loop execution time > 20 ms\n");
	
	return 0;
}

void LRTRobot13::Main()
{
	wdStart(_watchdog, sysClkRateGet() / RobotConfig::LOOP_RATE,
			TimeoutCallback, 0);
	
	// Redirect all prints to a file when console is blocked during matches
	if (DriverStation::GetInstance()->IsFMSAttached())
	{
		AsyncPrinter::RedirectToFile(RobotConfig::PRINT_FILE_PATH.c_str());
	}
	else
	{
		AsyncPrinter::RestoreToConsole();
	}
	
	// Update global robot state object
	RobotState::Update();
	
	// Update the Driver Station
	LRTDriverStation::Update();

	// Update the Brain
	Brain::Instance()->Update();
	
	// Update all components
	Component::UpdateAll();
	
	// Flush outputs to all actuators
	Actuator::UpdateAll();

	// Toggle compressor based on Driver Station switches
	if (DriverStation::GetInstance()->GetDigitalIn(DriverStationConfig::DigitalIns::COMPRESSOR))
	{
		Pneumatics::SetCompressor(true);
	}
	else
	{
		Pneumatics::SetCompressor(false);
	}
	
	// Utilities
	LCD::Instance()->RunOneCycle();
	Logger::Instance()->Run();
	
	wdCancel(_watchdog);
}

