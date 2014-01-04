#ifndef AUTOMATION_H_
#define AUTOMATION_H_

#include <vector>
#include <set>
#include <map>
#include "../ControlResource.h"

class Event;

using namespace std;

/*!
 * @brief Base class for all automation tasks.
 */
class Automation
{
public:
    /*!
     * @brief Creates an automation routine.
     * @param name the name of the routine
     * @param requiresAbortCycles whether the routine requires multiple cycles to abort
     * @param queueIfBlocked whether the routine should be queued if blocked
     * @param restartable whether the routine can be restarted
     */
	Automation(const char *name, bool requiresAbortCycles = false, bool queueIfBlocked = false, bool restartable = false);
	virtual ~Automation();
	
	/*!
	 * @brief Updates the task for one cycle, including calling Run().
	 * @return true if task has completed, false otherwise
	 */
	bool Update();
	
	/*!
	 * @brief Tries to start this task.
	 * @param trigger the event that fired
	 * @return if the task successfully started
	 */
	bool StartAutomation(Event *trigger = NULL);

	/*!
	 * @brief Tries to aborts this task.
	 * @param trigger the event that fired
	 * @return if the task successfully aborted
	 */
	bool AbortAutomation(Event *trigger = NULL);

	/*!
	 * @brief Sets the task to continue.
	 * @param trigger the event that fired
	 */
	void ContinueAutomation(Event *trigger);
	
	/*!
	 * @brief Allocates resources used by this task. Checks if this task is blocked.
	 * @return if this task is blocked
	 */
	bool CheckResources();
	
	/*!
	 * @brief Calls AllocateResource() on resources used in by this task.
	 */
	virtual void AllocateResources() = 0;
	
	/*!
	 * @brief Clears all resources allocated by this task.
	 */
	void DeallocateResources();
	
	/*!
	 * @brief Gets the task that allocated a resource.
	 * @param resource the resource to check
	 * @return the task that allocated the resource, or NULL if the resource is not allocated
	 */
	static bool GetAllocation(ControlResource resource);
	
	/*!
	 * @brief Gets whether a task can be restarted (started again when already running).
	 * @return whether the task can be restarted
	 */
	bool IsRestartable();

	/*!
	 * @brief Gets whether a task should continue to be started if blocked.
	 * @return whether the task should be queued if blocked
	 */
	bool QueueIfBlocked();
	
	/*!
	 * @brief Gets whether a task requires running cycles before aborting.
	 * @return whether the task requires abort cycles
	 */
	bool RequiresAbortCycles();
	
	/*!
	 * @brief Gets the name of the automation routine.
	 * @return the name of the routine
	 */
	const char* GetName();
	
	static vector<Automation*> automation_vector;
	
protected:
	/*!
	 * @brief Starts the task.
	 * @return true if task is able to start, false otherwise
	 */
	virtual bool Start() = 0;
	
	/*!
	 * @brief Aborts the task.
	 * @return true if task can be aborted, false otherwise
	 */
	virtual bool Abort() = 0;
	
	/*!
	 * @brief Runs one cycle of the task.
	 * @return true if completed, false otherwise
	 */
	virtual bool Run() = 0;

	/*!
	 * @brief Allocates a resource to be used.
	 * @param resource the resource to allocate
	 * @return if the allocation was successful
	 */
	bool AllocateResource(ControlResource resource);

	/*!
	 * @brief Gets whether this task is aborting (abort event fired and requires abort cycles).
	 * @return whether the task is aborting
	 */
	bool Aborting();
	
	/*!
	 * @brief Returns whether this task has been continued.
	 * @return whether this task has been continued
	 */
	bool Continued();

	/*!
	 * @brief Returns the event that started this task.
	 * @return the event that started this task, or NULL if this task was not started by an event
	 */
	Event* GetStartEvent();
	
	/*!
	 * @brief Returns the event that aborted this task.
	 * @return the event that aborted this task, or NULL if this task was not aborted by an event
	 */
	Event* GetAbortEvent();
	
	/*!
	 * @brief Returns the event that continued this task.
	 * @return the event that continued this task, or NULL if this task has not been continued
	 */
	Event* GetContinueEvent();
	
private:
	static map<ControlResource, int> allocated;
	vector<ControlResource> resources;
	Event *m_startEvent;
	Event *m_abortEvent;
	Event *m_continueEvent;
	bool m_aborting;
	bool m_restartable;
	bool m_queueIfBlocked;
	bool m_requiresAbortCycles;
	const char *m_name;
};

#endif
