#include "Sequential.h"
#include <math.h>

Sequential::Sequential(const char *name, bool restartable) :
	Automation(name, restartable)
{
	started = false;
}

Sequential::Sequential(const char *name, vector<Automation*> sequence, bool restartable) :
	Automation(name, restartable)
{
	routines = sequence;
	started = false;
}

Sequential::~Sequential()
{
	
}

void Sequential::AllocateResources()
{
	if (!queued.empty())
	{
		queued.front()->GetBlockingTasks().clear();
		queued.front()->AllocateResources();
		GetBlockingTasks().insert(queued.front()->GetBlockingTasks().begin(), queued.front()->GetBlockingTasks().begin());
	}
}

Automation::Status Sequential::Start(Event *trigger)
{
	if (routines.empty())
		return REJECTED;
	while (!queued.empty())
		queued.pop();
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
		queued.push(*it);
	startEvent = trigger;
	started = false;
	return SUCCESS;
}

bool Sequential::Run()
{
	if (!started && ContinueNextStep())
	{
		Status res = queued.front()->Start(startEvent);
		if (res == SUCCESS)
			started = true;
		else if (res == REJECTED)
			return true;
	}
	if (started)
	{
		bool completed = queued.front()->Run();
		if (completed)
		{
			queued.pop();
			started = false;
		}
	}
	if (queued.empty())
		return true;
	return false;
}

Automation::Status Sequential::Abort(Event *trigger)
{
	if (!queued.empty())
	{
		Status res = queued.front()->Abort(trigger);
		if (res == SUCCESS)
		{
			while (!queued.empty())
				queued.pop();
		}
		return res;
	}
	return SUCCESS;
}

void Sequential::AddAutomation(Automation *automation)
{
	routines.push_back(automation);
}

void Sequential::AddAutomation(vector<Automation*> automation)
{
	routines.reserve(routines.size() + automation.size());
	routines.insert(routines.end(), automation.begin(), automation.end());
}

void Sequential::ClearSequence()
{
	routines.clear();
}

bool Sequential::ContinueNextStep()
{
	return true;
}
