#ifndef REPEATING_H_
#define REPEATING_H_

#include "Automation.h"
#include <queue>
#include <vector>

using namespace std;

/*!
 * @brief Container for a repeating automation routine.
 */
class Repeating : public Automation
{
public:
	/*!
	 * @brief Creates a repeating routine.
	 * @param name the name of the routine
	 * @param routine the routine to repeat
	 * @param times the number of times to repeat, or 0 if the routine should be repeated until aborted
     * @param queueIfBlocked whether the routine should be queued if blocked
	 * @param restartable whether the routine can be restarted
	 */
	Repeating(const char *name, Automation *routine, int times = 0, bool queueIfBlocked = false, bool restartable = false);
	virtual ~Repeating();

	virtual bool Start();
	virtual bool Abort();
	virtual bool Run();
	virtual void AllocateResources();
	
protected:
	/*!
	 * @brief Checks whether the the routine can be repeated this cycle.
	 * @return whether the routine can be repeated
	 */
	virtual bool Repeat();
	
	/*!
	 * @brief Checks if the routine is done repeating
	 * @return if the routine is done repeating
	 */
	virtual bool FinishRepeat();
	
private:
	Automation *m_routine;
	int m_times;
	int m_count;
	bool m_completed;
};

#endif
