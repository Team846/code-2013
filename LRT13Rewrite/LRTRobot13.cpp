#include "LRTRobot13.h"

#include "Output/Output.h"
#include "Output/AsyncCANJaguar.h"
#include "Output/Pneumatics.h"
#include "RobotState.h"

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
	
	AsyncPrinter::Finalize();
	ConfigManager::Finalize();
	LCD::Finalize();
	Pneumatics::DestroyCompressor();
}

void LRTRobot13::RobotInit()
{
	// Initialize Robot State
	RobotState::Initialize();
	
	// Initialize Utilities
	AsyncPrinter::Initialize();
	LCD::Instance()->Start();
	
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

	ConfigManager::Instance()->ConfigureAll();
}

static int TimeoutCallback(...)
{
	printf("Main loop execution time > 20ms\n");
	
	return 0;
}

void LRTRobot13::Main()
{
	wdStart(_watchdog, sysClkRateGet() / RobotConfig::LOOP_RATE,
			TimeoutCallback, 0);

	RobotState::Update();
	
	// Update all output resources
	for (vector<Output*>::iterator it = Output::output_vector.begin(); it < Output::output_vector.end(); it++)
	{
		(*it)->Update();
	}

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

