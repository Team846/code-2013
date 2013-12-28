#include "Sequential.h"

Sequential::Sequential(const char *name, bool queueIfBlocked, bool restartable) :
	Automation(name, false, queueIfBlocked, restartable)
{
	started = false;
}

Sequential::Sequential(const char *name, vector<Automation*> sequence, bool queueIfBlocked, bool restartable) :
	Automation(name, false, queueIfBlocked, restartable)
{
	routines = sequence;
	started = false;
}

Sequential::~Sequential()
{
	
}

void Sequential::AllocateResources()
{
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
		(*it)->AllocateResources();
}

bool Sequential::Start()
{
	if (routines.empty())
		return false;
	while (!queued.empty())
		queued.pop();
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
		queued.push(*it);
	started = false;
	return true;
}

bool Sequential::Run()
{
	if (!started && ContinueNextStep())
	{
		bool res = queued.front()->StartAutomation(GetStartEvent());
		if (res)
			started = true;
		else
			return true;
	}
	if (started)
	{
		bool completed = queued.front()->Update();
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

bool Sequential::Abort()
{
	if (!queued.empty())
	{
		bool res = queued.front()->AbortAutomation(GetAbortEvent());
		if (res)
		{
			while (!queued.empty())
				queued.pop();
		}
		return res;
	}
	return true;
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
