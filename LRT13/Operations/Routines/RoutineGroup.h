#ifndef ROUTINE_GROUP_H_
#define ROUTINE_GROUP_H_

#include "Routine.h"

class RoutineGroup : public Routine
{
public:
	RoutineGroup(vector<Routine*> routines);
	virtual ~RoutineGroup();
	
	virtual void Run();
	
	virtual bool Completed();
private:
	vector<Routine*> m_routines;
};

#endif
