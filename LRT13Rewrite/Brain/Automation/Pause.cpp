#include "Pause.h"

Pause::Pause(double time) :
	Automation("Pause")
{
	m_time = time;
	timingCycles = false;
}

Pause::Pause(int cycles) :
	Automation("Pause")
{
	m_cycles = cycles;
	timingCycles = true;
}

void Pause::AllocateResources()
{
	
}

bool Pause::Start()
{
	if (timingCycles)
	{
		m_currentCycles = 0;
	}
	else
	{
		m_timer.Reset();
		m_timer.Start();
	}
	return true;
}

bool Pause::Run()
{
	if (timingCycles)
		m_currentCycles++;
	return timingCycles ? m_timer.Get() > m_time : m_currentCycles >= m_cycles;
}

bool Pause::Abort()
{
	if (timingCycles)
	{
		m_currentCycles = 0;
	}
	else
	{
		m_timer.Stop();
	}
	return true;
}
