#ifndef ARMS_H_
#define ARMS_H_

#include "Routine.h"
#include "Timer.h"

class Arms : public Routine
{
public:
	Arms(bool up);
	
	virtual void Run();
	
	virtual bool Completed();
private:
	bool m_up;
};

#endif
