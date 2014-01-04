#include "LRTRobot13.h"

#include "Brain/Brain.h"
#include "Components/Component.h"
#include "Actuators/Actuator.h"
#include "Actuators/AsyncCANJaguar.h"
#include "Actuators/Pneumatics.h"
#include "RobotState.h"
#include "DriverStation/LRTDriverStation.h"
#include "ComponentData/ComponentData.h"
#include "Sensors/RobotLocation.h"

#include "Config/ConfigRuntime.h"
#include "Config/ConfigPortMappings.h"
#include "Config/RobotConfig.h"
#include "Config/DriverStationConfig.h"
#include "Utils/LCD.h"
#include "Logging/Logger.h"
#include "Communication/LiveNetworkSender.h"
#include "Communication/OffboardCommunication.h"

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
	
	// Delete all singletons
	Component::DestroyComponents();
	ComponentData::Finalize();
	ConfigPortMappings::Finalize();
	ConfigRuntime::Finalize();
	Logger::Finalize();
	LiveNetworkSender::Finalize();
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
	
	// Create ComponentData
	AsyncPrinter::Println("Creating ComponentData...");
	ComponentData::Initialize();
	
	// Create all components
	AsyncPrinter::Println("Creating Components...");
	Component::CreateComponents();
	
	// Initialize the Brain
	AsyncPrinter::Println("Initializing Brain...");
	Brain::Initialize();
	
	// Start AsyncCANJaguar tasks
	AsyncPrinter::Println("Starting AsyncCANJaguar Tasks...");
	for (vector<AsyncCANJaguar*>::iterator it = AsyncCANJaguar::jaguar_vector.begin(); it < AsyncCANJaguar::jaguar_vector.end(); it++)
	{
		(*it)->Start();
	}

	// Create and start compressor
	AsyncPrinter::Println("Creating Pneumatics Compressor...");
	Pneumatics::CreateCompressor();

	// Initialize localization
	AsyncPrinter::Println("Initializing Robot Localization...");
	RobotLocation::Initialize();
	
	// Initialize offboard communication
	AsyncPrinter::Println("Initializing Offboard Communication...");
	OffboardCommunication::Initialize();

	// Initialize the Logger
	AsyncPrinter::Println("Initializing Logger...");
	Logger::Instance()->Initialize();
	
	// Initialize the LiveNetworkSender
	AsyncPrinter::Println("Initializing LiveNetworkSender...");
	LiveNetworkSender::Initialize();
	
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
	
	// Zero robot location if enabled
	if (RobotState::Instance().GameMode() != RobotState::DISABLED && RobotState::Instance().LastGameMode() == RobotState::DISABLED)
	{
		RobotLocation::Instance()->Zero();
	}
	
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
	
	// Check for runtime configuration file changes
	if (RobotState::Instance().GameMode() == RobotState::DISABLED)
	{
		ConfigRuntime::Instance()->CheckForFileUpdates();
	}
	
	// Utilities
	LCD::Instance()->RunOneCycle();
	Logger::Instance()->Run();
	LiveNetworkSender::Instance()->Run();
	
	// Reset ComponentData command fields
	ComponentData::ResetAllCommands();
	
	wdCancel(_watchdog);
}

