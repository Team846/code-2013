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
			
					printf("Darn it\n");
					GetWatchdog().SetEnabled(false);
			
					// first and one-time initialization
					RobotInit();
		
	while(true)
	{
		Tick();
		
		Wait(1.0);
	}
}

void LRTRobotBase::Tick()
{
	printf("LRTRobotBase::Tick(): Override me!");
}

bool LRTRobotBase::IsRunning()
{
	return true;
}
