#include "Parallel.h"
#include <math.h>

Parallel::Parallel(const char *name, bool restartable) :
	Automation(name, restartable)
{
}

Parallel::Parallel(const char *name, vector<Automation*> sequence, bool restartable) :
	Automation(name, restartable)
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
		(*it)->GetBlockingTasks().clear();
		(*it)->AllocateResources();
		GetBlockingTasks().insert((*it)->GetBlockingTasks().begin(), (*it)->GetBlockingTasks().begin());
	}
}

Automation::Status Parallel::Start(Event *trigger)
{
	Status successLevel = SUCCESS;
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
	{
		Status ret = (*it)->Start(trigger);
		if (ret > successLevel)
			successLevel = ret;
	}
	return successLevel;
}

bool Parallel::Run()
{
	bool completed = true;
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
	{
		if(!(*it)->Run())
			completed = false;
	}
	return completed;
}

Automation::Status Parallel::Abort(Event *trigger)
{
	Status successLevel = SUCCESS;
	for (vector<Automation*>::iterator it = routines.begin(); it < routines.end(); it++)
	{
		Status ret = (*it)->Abort(trigger);
		if (ret > successLevel)
			successLevel = ret;
	}
	return successLevel;
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
