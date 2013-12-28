#ifndef PAUSE_H_
#define PAUSE_H_

#include "Automation.h"
#include "Timer.h"

class Pause : public Automation
{
public:
	Pause(double time);
	Pause(int cycles);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
	
private:
	bool timingCycles;
	double m_time;
	int m_cycles;
	Timer m_timer;
	int m_currentCycles;
};

#endif
