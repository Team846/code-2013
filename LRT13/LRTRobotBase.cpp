#include "LRTRobotBase.h"

LRTRobotBase::LRTRobotBase()
{
}

LRTRobotBase::~LRTRobotBase()
{
	printf("robot destructing\n");
}

void LRTRobotBase::StartCompetition()
{
	//Diagnostic: Print the task name.
	printf("vxWorks task: %s\n", m_task->GetName());

	GetWatchdog().SetEnabled(false);

	// first and one-time initialization
	RobotInit();
		
	Run();
}

void LRTRobotBase::Run()
{
	printf("LRTRobotBase::Run(): Override me!");
}

bool LRTRobotBase::IsRunning()
{
	return true;
}
