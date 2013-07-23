#include "AsyncProcess.h"

AsyncProcess::AsyncProcess(const char * taskName, INT32 priority)
{
	m_task = new Task(taskName, (FUNCPTR)_TASK_ENTRY, priority);
	m_isRunning = false;
	
	m_quittingSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
}

AsyncProcess::~AsyncProcess()
{
	Abort(0);

	DELETE(m_task);
	
	semDelete(m_quittingSem);
}

void AsyncProcess::Start()
{
	m_task->Start((UINT32)this);
}

bool AsyncProcess::IsRunning()
{
	return m_isRunning;
}

void AsyncProcess::Abort(int code, double waitSeconds)
{
	m_isRunning = false;
	
	// wait for task to end
	semTake(m_quittingSem, (int)(sysClkRateGet() * waitSeconds));
	
	if(m_task != NULL)
		m_task->Stop();
	
	if(code != 0)
	{
		printf("[WARNING] Task \"%s\" aborted with status %d.\n", m_task->GetName(), code);
	}
}

void AsyncProcess::preTick() { }

void AsyncProcess::_TASK_ENTRY(void* asyncProcessInstance)
{
	AsyncProcess* _instance = static_cast<AsyncProcess*>(asyncProcessInstance);
	
	_instance->m_isRunning = true;
	
	while(_instance->m_isRunning)
	{
		_instance->preTick();
		_instance->Tick();
	}

	//_instance->m_isRunning = false;
	semGive(_instance->m_quittingSem);
}
