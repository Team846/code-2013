#include "Profiler.h"

stack<pair<string, double> > Profiler::m_activites;
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
	
	pair<string, uint64_t> p;
	
	p.first = name;
	p.second = now;
	
	m_activites.push(p);
}

double Profiler::EndLast()
{
	pair<string, double> p = m_activites.top();
	
	m_activites.pop();
	
	double timeTook = Timer::GetFPGATimestamp() - p.second;
	
	pair<double, int> timePair = m_averageTime[p.first];
	m_averageTime[p.first].first = (m_averageTime[p.first].first + timeTook) / m_averageTime[p.first].second;
	++m_averageTime[p.first].second;
	
	return timeTook;
}

