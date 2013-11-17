#include "Stopwatch.h"

Stopwatch::Stopwatch()
{
	m_begin = 0.0;
	m_end = 0.0;
	m_totalTime = 0.0;
	m_stopped = true;
}

void Stopwatch::Start()
{
	if(!m_stopped)
	{
		AsyncPrinter::Println("Stopwatch::Start(): Already started!");
		return;
	}
	
	m_stopped = false;
	
	m_begin = Timer::GetFPGATimestamp();
}

void Stopwatch::Stop()
{
	if(m_stopped)
	{
		AsyncPrinter::Println("Stopwatch::Stop(): Never started!");
		return;
	}
	
	m_end = Timer::GetFPGATimestamp();
	
	m_totalTime += m_end - m_begin;
	
	m_stopped = true;
}

void Stopwatch::Reset()
{
	m_stopped = true;
	
	m_totalTime = 0.0;

	m_begin = 0.0;
	m_end = 0.0;
}

void Stopwatch::Restart()
{
	Reset();
	Start();
}

bool Stopwatch::Running()
{
	return !m_stopped;
}

double Stopwatch::TotalElapsedMinutes()
{
	return TotalElapsedSeconds() / 60.0;
}

double Stopwatch::TotalElapsedSeconds()
{
	return m_stopped ? (m_totalTime) : (Timer::GetFPGATimestamp() - m_begin + m_totalTime);
}

double Stopwatch::TotalElapsedMilliseconds()
{
	return TotalElapsedSeconds() * 1000.0;
}
