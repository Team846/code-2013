#ifndef EVENT_H_
#define EVENT_H_

#include <vector>
#include "../Automation/Automation.h"

using namespace std;

/*!
 * @brief Base class for events, which are used to control automation routines.
 */
class Event
{
public:
	Event();
	virtual ~Event();
	
	/*!
	 * @brief Checks if the condition required to fire this event is met, regardless of the last cycle.
	 * @return if the condition is met
	 */
	
	virtual bool CheckCondition() = 0;
	/*!
	 * @brief Checks if the event fired this cycle (condition not true last cycle but true this cycle).
	 * @return if the event fired
	 */
	virtual bool Fired();
	
	/*!
	 * @brief Updates the event. Should be called at the end of the cycle.
	 */
	virtual void Update();
	
	/*!
	 * @brief Adds an automation routine to be started when this event fires.
	 * @param routine the routine to add
	 */
	void AddStartListener(Automation* routine);
	/*!
	 * @brief Adds an automation routine to be aborted when this event fires.
	 * @param routine the routine to add
	 */
	void AddAbortListener(Automation* routine);
	/*!
	 * @brief Adds an automation routine to be continued when this event fires.
	 * @param routine the routine to add
	 */
	void AddContinueListener(Automation* routine);
	/*!
	 * @brief Gets the automation routines that are started by this event.
	 * @return a vector of automation routines to be started
	 */
	vector<Automation*>& GetStartListeners();
	/*!
	 * @brief Gets the automation routines that are aborted by this event.
	 * @return a vector of automation routines to be aborted
	 */
	vector<Automation*>& GetAbortListeners();
	/*!
	 * @brief Gets the automation routines that are continued by this event.
	 * @return a vector of automation routines to be continued
	 */
	vector<Automation*>& GetContinueListeners();
	
	static vector<Event*> event_vector;
	
private:
	vector<Automation*> start_listeners;
    vector<Automation*> abort_listeners;
    vector<Automation*> continue_listeners;
    bool lastFired;
};

#endif
