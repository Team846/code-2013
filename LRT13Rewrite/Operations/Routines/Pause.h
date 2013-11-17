#ifndef PAUSE_H_
#define PAUSE_H_

#include "Routine.h"
#include "Timer.h"

class Pause : public Routine
{
public:
	Pause(double time);
	
	virtual void Run();
	
	virtual bool Completed();
private:
	double m_time;
	
	Timer m_timer;
};

#endif
