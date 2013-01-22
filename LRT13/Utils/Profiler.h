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
	 * @brief Ends the last created activity.
	 * @returns The time, in microseconds, that the activity took.
	 */
	static double EndLast();
	
private:
	static stack<pair<string, double> > m_activites;
	static map<string, pair<double, int> > m_averageTime;
};

#endif
