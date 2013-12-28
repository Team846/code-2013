#include "Repeating.h"

Repeating::Repeating(const char *name, Automation *routine, int times, bool queueIfBlocked, bool restartable) :
	Automation(name, false, queueIfBlocked, restartable)
{
	m_routine = routine;
	m_times = times;
	m_count = 0;
	m_completed = false;
}

Repeating::~Repeating()
{
	
}

void Repeating::AllocateResources()
{
	m_routine->AllocateResources();
}

bool Repeating::Start()
{
	m_count = 0;
	m_completed = false;
	return m_routine->StartAutomation(GetStartEvent());
}

bool Repeating::Run()
{
	if (!m_completed)
		m_completed = m_routine->Update();
	if (m_completed)
	{
		if (Repeat() && !FinishRepeat())
		{
			m_routine->StartAutomation(GetStartEvent());
			m_count++;
			m_completed = false;
		}
		else if (FinishRepeat())
			return true;
	}
	return false;
}

bool Repeating::Abort()
{
	return m_routine->AbortAutomation(GetAbortEvent());
}

bool Repeating::Repeat()
{
	return true;
}

bool Repeating::FinishRepeat()
{
	return m_count != 0 ? m_count >= m_times : false;
}
