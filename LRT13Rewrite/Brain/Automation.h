#ifndef AUTOMATION_H_
#define AUTOMATION_H_

#include <vector>
#include <set>
#include <map>

class Event;
class ComponentData;

using namespace std;

/*!
 * @brief Base class for all automation tasks.
 */
class Automation
{
public:
    enum Status 
    {
        SUCCESS,
        REJECTED,
        IN_PROGRESS,
        COMPLETED,
        RESOURCE_BUSY,
        CLEANING_UP,
        WAIT
    };
    
	Automation();
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
	 * @brief Returns a list of tasks that hold the required resources for the current cycle of this task.
	 * @return set containing blocking tasks
	 */
	set<Automation*>& GetBlockingTasks();

	/*!
	 * @brief Gets the task allocating the given resource.
	 * @param resource the resource to check
	 * @return the allocating task, or NULL if the resource is not allocated
	 */
	static Automation* GetAllocation(ComponentData *resource);
	
	/*!
	 * @brief Clears all resource allocations.
	 */
	static void DeallocateAll();
	
protected:
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
	 * @brief Allocates the resource to this task.
	 * @param resource the resource to allocate
	 */
	bool AllocateResource(ComponentData* resource);
	
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
	static map<ComponentData*, Automation*> allocated;
	set<Automation*> blocking;
	Event *m_continueEvent;
};

#endif
