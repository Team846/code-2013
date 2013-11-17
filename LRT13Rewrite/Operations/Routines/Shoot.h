#ifndef SHOOT_H_
#define SHOOT_H_

#include "Routine.h"
#include "Timer.h"

class Shoot : public Routine
{
public:
	Shoot(int count, double timeout = 0.0);
	
	virtual void Run();
	
	virtual void Stop();
	
	virtual bool Completed();
private:
	int m_count;
	double m_timeout;
	
	Timer m_timer;
	
	int m_startingCount;
};

#endif
