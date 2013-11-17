#ifndef COLLECT_H_
#define COLLECT_H_

#include "Routine.h"

class Collect : public Routine
{
public:
	Collect(bool collect);
	
	virtual void Run();
	
	virtual bool Completed();
private:
	bool m_collect;
};

#endif
