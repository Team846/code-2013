#ifndef AUTOMATION_H_
#define AUTOMATION_H_

#include <vector>
#include <set>
#include <map>

class Event;

using namespace std;

/*!
 * @brief Base class for all automation tasks.
 */
class Automation
{
public:
    enum Status 
    {
        SUCCESS = 0,
        WAIT = 1,
        CLEANING_UP = 2,
        REJECTED = 3,
        IN_PROGRESS = 4,
        COMPLETED = 5,
        RESOURCE_BUSY = 6
    };
    enum Resource
    {
    	DRIVE,
    	TURN,
    	COLLECTOR_SLIDE,
    	COLLECTOR_ROLLERS,
    	ANGLE
    };
    
    /*!
     * @brief Creates an automation routine.
     * @param name the name of the routine
     * @param restartable whether the routine can be restarted
     */
	Automation(const char *name, bool restartable = false);
	virtual ~Automation();
	
	/*!
	 * @brief Tries to run one cycle if required resources are not allocated.
	 * @return COMPLETED if run completes the task, IN_PROGRESS if run is not complete, or RESOURCE_BUSY if required resources have already been allocated
	 */
	Status Update();
	
	/*!
	 * @brief Called the cycle that the start event is fired.
	 * @param trigger the event that fired
	 * @return SUCCESS if task is able to start, REJECTED if task cannot start, or WAIT if task should be queued for starting later
	 */
	virtual Status Start(Event *trigger) = 0;
	
	/*!
	 * @brief Called the cycle that the abort event is fired.
	 * @param trigger the event that fired
	 * @return SUCCESS if task is able to abort, REJECTED if task cannot abort, or CLEANING_UP if task requires multiple cycles to clean up before aborting
	 */
	virtual Status Abort(Event *trigger) = 0;

	/*!
	 * @brief Sets the task to continue.
	 */
	void Continue(Event *trigger);
	
	/*!
	 * @brief Calls AllocateResource() on resources used in a cycle.
	 */
	virtual void AllocateResources() = 0;
	
	/*!
	 * @brief Runs one cycle of the task.
	 * @return true if completed, false otherwise
	 */
	virtual bool Run() = 0;
	
	/*!
	 * @brief Returns a list of tasks that hold the required resources for the current cycle of this task.
	 * @return set containing blocking tasks
	 */
	set<Automation*>& GetBlockingTasks();

	/*!
	 * @brief Gets the task allocating the given resource.
	 * @param resource the resource to check
	 * @return the allocating task, or NULL if the resource is not allocated
	 */
	static Automation* GetAllocation(Resource resource);
	
	/*!
	 * @brief Clears all resource allocations.
	 */
	static void DeallocateAll();
	
	/*!
	 * @brief Gets whether a task can be restarted (started again when already running).
	 * @return whether the task can be restarted
	 */
	bool IsRestartable();
	
	/*!
	 * @brief Gets the name of the automation routine.
	 * @return the name of the routine
	 */
	const char* GetName();
	
protected:
	/*!
	 * @brief Allocates the resource to this task.
	 * @param resource the resource to allocate
	 */
	bool AllocateResource(Resource resource);
	
	/*!
	 * @brief Returns whether this task has been continued.
	 * @return whether this task has been continued
	 */
	bool Continued();

	/*!
	 * @brief Returns the event that continued this task.
	 * @return the event that continued this task, or NULL if this task has not been continued.
	 */
	Event* GetContinueEvent();
	
private:
	static map<Resource, Automation*> allocated;
	set<Automation*> blocking;
	Event *m_continueEvent;
	bool m_restartable;
	const char *m_name;
};

#endif
