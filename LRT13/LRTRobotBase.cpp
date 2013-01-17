#include "LRTRobotBase.h"

LRTRobotBase::LRTRobotBase()
{
	printf("ctor");
}

LRTRobotBase::~LRTRobotBase()
{
	
}

void LRTRobotBase::StartCompetition()
{
	while(true)
	{
		Tick();
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
