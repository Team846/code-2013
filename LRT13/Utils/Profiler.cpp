#include "Profiler.h"

map<string, double> Profiler::m_activities;
map<string, pair<double, int> > Profiler::m_averageTime;

void Profiler::BeginActivity(string name)
{
	if(m_averageTime.find(name) == m_averageTime.end())
	{
		// create the activity
		pair<double, int> p;
		
		p.first = 0.0;
		p.second = 1;
		
		m_averageTime[name] = p;
	}
	
	double now = Timer::GetFPGATimestamp();
	
	m_activities[name] = now;
}

double Profiler::End(string name)
{
	double startTime = m_activities[name];
	
	m_activities.erase(name);
	
	double timeTook = Timer::GetFPGATimestamp() - startTime;
	
	pair<double, int> timePair = m_averageTime[name];
	m_averageTime[name].first = (m_averageTime[name].first + timeTook) / m_averageTime[name].second;
	++m_averageTime[name].second;
	
	return timeTook;
}
