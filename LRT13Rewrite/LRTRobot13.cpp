#include "LRTRobot13.h"

#include "Brain/Brain.h"
#include "Components/Component.h"
#include "Actuators/Actuator.h"
#include "Actuators/AsyncCANJaguar.h"
#include "Actuators/Pneumatics.h"
#include "RobotState.h"

#include "Config/ConfigRuntime.h"
#include "Config/ConfigPortMappings.h"
#include "Config/RobotConfig.h"
#include "Config/DriverStationConfig.h"
#include "Utils/LCD.h"
#include "Brain/Events/Event.h"

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
	Pneumatics::DestroyCompressor();
	LCD::Finalize();
	AsyncPrinter::Finalize();
	RobotState::Finalize();
}

void LRTRobot13::RobotInit()
{
	// Initialize global robot state object
	RobotState::Initialize();
	
	// Initialize Utilities
	AsyncPrinter::Initialize();
	LCD::Instance()->Start();
	
	// Read port mappings
	ConfigPortMappings::Instance()->Load();
	
	// Create all components
	Component::CreateComponents();
	
	// Initialize the Brain
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
	
	Pneumatics::CreateCompressor();

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

	if (DriverStation::GetInstance()->IsFMSAttached())
	{
		AsyncPrinter::RedirectToFile("/stdout.out");
	}
	else
	{
		AsyncPrinter::RestoreToConsole();
	}
	
	// Update global robot state object
	RobotState::Update();
	
	// Update all components
	Component::UpdateAll();
	
	// Flush outputs to all actuators
	Actuator::UpdateAll();

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
	
	Event::UpdateEvents();
	
	wdCancel(_watchdog);
}

/*!
 * Pre-defined macro is:
 * START_ROBOT_CLASS(class);
 * 
 * The reason for expanding to macro is to make it easier to perform unit tests.
 */
RobotBase* FRC_userClassFactory()
{
	return new LRTRobot13();
}

extern "C"
{
	INT32 FRC_UserProgram_StartupLibraryInit()
	{
		RobotBase::startRobotTask((FUNCPTR) FRC_userClassFactory);
		return 0;
	}
}

