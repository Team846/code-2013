#include "Pause.h"

Pause::Pause(double time)
{
	m_time = time;
}

void Pause::Run()
{
	m_timer.Start();
}

bool Pause::Completed()
{
	return m_timer.Get() > m_time;
}
