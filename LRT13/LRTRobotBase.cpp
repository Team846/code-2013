#include "LRTRobotBase.h"
#include <sysLib.h>

LRTRobotBase::LRTRobotBase()
: m_loopSynchronizer((TimerEventHandler) LRTRobotBase::ReleaseLoop, this)
{
	m_loopSynchSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	semTake(m_loopSynchSem, WAIT_FOREVER);
}

LRTRobotBase::~LRTRobotBase()
{
	printf("robot destructing\n");
}

void LRTRobotBase::StartCompetition()
{
	//Diagnostic: Print the task name.
	taskDelay(sysClkRateGet());//wait 1 sec, see if this fixes the issue
	printf("vxWorks task: %s\n", m_task->GetName()); //Maybe this line was crashing it...

	GetWatchdog().SetEnabled(false);
	m_loopSynchronizer.StartPeriodic(1.0/50.0);

	// first and one-time initialization
	RobotInit();
//	
//	//start the thread
	Run();
}

void LRTRobotBase::Run()
{
	while(true)
	{
		semTake(m_loopSynchSem, WAIT_FOREVER);
		Tick();
	}
}

void LRTRobotBase::Tick()
{
	printf("LRTRobotBase::Run(): Override me!");
}

bool LRTRobotBase::IsRunning()
{
	return true;
}

void LRTRobotBase::ReleaseLoop(void *param)
{
	LRTRobotBase *robot = (LRTRobotBase *) param;
	semGive(robot->m_loopSynchSem);
}
