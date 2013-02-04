#ifndef ASYNC_PROCESS_H_
#define ASYNC_PROCESS_H_

#include "WPILib.h"
#include "sysLib.h"

#include "../Utils/Defines.h"

/*!
 * @brief Base class for asynchronous processes. Asynchronous processes run in a free loop.
 * @author Tony Peng
 */

class AsyncProcess
{
public:
	AsyncProcess(const char * taskName, INT32 priority=Task::kDefaultPriority);
	virtual ~AsyncProcess();
	
	void Start();
	void Abort(int code=0, double waitSeconds=0);
	
	bool IsRunning();
protected:
	virtual void preTick();
	virtual INT32 Tick();
private:
	static void _TASK_ENTRY(void* asyncProcessInstance);

	Task* m_task;
	bool m_isRunning;
	
	SEM_ID m_quittingSem;
};

#endif
