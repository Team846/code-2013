#include "Parallel.h"

Parallel::Parallel(const char *name, bool queueIfBlocked, bool restartable) :
	Automation(name, false, queueIfBlocked, restartable)
{
}

Parallel::Parallel(const char *name, vector<Automation*> sequence, bool queueIfBlocked, bool restartable) :
	Automation(name, false, queueIfBlocked, restartable)
{
	routines = sequence;
}

Parallel::~Parallel()
{
	
}

void Parallel::AllocateResources()
{
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
	{
		(*it)->AllocateResources();
	}
}

bool Parallel::Start()
{
	bool success = true;
	running.clear();
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
	{
		bool ret = (*it)->StartAutomation(GetStartEvent());
		if (!ret)
			success = false;
	}
	if (success)
	{
		running = routines;
	}
	return success;
}

bool Parallel::Run()
{
	bool completed = true;
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
	{
		if(!(*it)->Update())
		{
			completed = false;
		}
		else
		{
			running.erase(it++);
			it--;
		}
	}
	return completed;
}

bool Parallel::Abort()
{
	bool success = true;
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
	{
		bool ret = (*it)->AbortAutomation(GetAbortEvent());
		if (!ret)
			success = false;
		else
		{
			running.erase(it++);
			it--;
		}
	}
	return success;
}

void Parallel::AddAutomation(Automation *automation)
{
	routines.push_back(automation);
}

void Parallel::AddAutomation(vector<Automation*> automation)
{
	routines.reserve(routines.size() + automation.size());
	routines.insert(routines.end(), automation.begin(), automation.end());
}
	
void Parallel::ClearAutomation()
{
	routines.clear();
}
