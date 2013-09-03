#include "RoutineGroup.h"
#include <math.h>

RoutineGroup::RoutineGroup(vector<Routine*> routines)
{
	m_routines = routines;
}

RoutineGroup::~RoutineGroup()
{
	for (vector<Routine*>::iterator it = m_routines.begin(); it < m_routines.end(); it++)
	{
		delete *it;
	}
}

void RoutineGroup::Run()
{
	for (vector<Routine*>::iterator it = m_routines.begin(); it < m_routines.end(); it++)
	{
		(*it)->Run();
	}
}

bool RoutineGroup::Completed()
{
	bool completed = true;
	for (vector<Routine*>::iterator it = m_routines.begin(); it < m_routines.end(); it++)
	{
		if (!(*it)->Completed())
			completed = false;
	}
	return completed;
}
