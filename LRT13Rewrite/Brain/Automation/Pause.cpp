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

Automation::Status Pause::Start(Event *trigger)
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
	return SUCCESS;
}

bool Pause::Run()
{
	if (timingCycles)
		m_currentCycles++;
	return timingCycles ? m_timer.Get() > m_time : m_currentCycles >= m_cycles;
}

Automation::Status Pause::Abort(Event *trigger)
{
	if (timingCycles)
	{
		m_currentCycles = 0;
	}
	else
	{
		m_timer.Stop();
	}
	return SUCCESS;
}
