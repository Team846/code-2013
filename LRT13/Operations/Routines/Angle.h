#ifndef ANGLE_H_
#define ANGLE_H_

#include "Routine.h"
#include "Timer.h"

class Angle : public Routine
{
public:
	Angle(bool high);
	
	virtual void Run();
	
	virtual void Stop();
	
	virtual bool Completed();
private:
	bool m_high;
};

#endif
