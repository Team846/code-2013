#ifndef PROFILER_H_
#define PROFILER_H_

#include <map>
#include <stack>
#include <string>
#include <Timer.h>

using namespace std;

/*!
 * @brief Provides a method to profile activities
 * @author Tony Peng
 */
class Profiler
{
public:
	/*!
	 * @brief Begins an activity called name.
	 */
	static void BeginActivity(string name);
	
	/*!
	 * @brief Ends an activity.
	 * @returns The time, in seconds, that the activity took.
	 */
	static double End(string name);
	
	/*!
	 * 
	 */
	static double GetLastTime(string name);
	
	static map<string, double>* GetLastTimes();
	
private:
	static map<string, double> m_activities;
	static map<string, double> m_lastTimes;
	static map<string, pair<double, int> > m_averageTime;
};

#endif
