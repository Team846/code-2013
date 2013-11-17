#ifndef SYNC_PROCESS_H_
#define SYNC_PROCESS_H_

#include <WPILib.h>
#include <semLib.h>

#include "AsyncProcess.h"

/*!
 * @brief Base class for synchronized processes. Synchronized processes wait for a signal before running a Tick.
 * @author Tony Peng, Raphael Chang
 */

class SynchronizedProcess : public AsyncProcess
{
public:
	SynchronizedProcess(const char * taskName, INT32 priority=Task::kDefaultPriority);
	SynchronizedProcess(const char * taskName, SEM_ID syncSem, INT32 priority=Task::kDefaultPriority);
	virtual ~SynchronizedProcess();
	/*!
	 * @brief Gives a semaphore which allows Tick() to be called
	 */
	void RunOneCycle();
//protected:
	/*!
	 * @brief Takes a semaphore which blocks Tick() until RunOneCycle() is called
	 */
	virtual void preTick();
private:
	SEM_ID m_syncSem;
};

#endif
