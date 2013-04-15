#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <Timer.h>

#include "Utils/AsyncPrinter.h"

class Stopwatch
{
public:
	Stopwatch();
	
	void Start();
	void Stop();
	void Restart();
	void Reset();
	
	double TotalElapsedMinutes();
	double TotalElapsedSeconds();
	double TotalElapsedMilliseconds();
private:
	double m_begin;
	double m_end;
	
	double m_totalTime;
	
	bool m_stopped;
};

#endif
