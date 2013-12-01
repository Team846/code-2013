#ifndef EVENT_H_
#define EVENT_H_

#include <vector>
#include "../Automation.h"

using namespace std;

/*!
 * @brief Base class for events, which are used to control automation routines.
 */
class Event
{
public:
	Event();
	virtual ~Event();
	
	virtual bool Fired() = 0;
	
	virtual void Update() = 0;
	
	void AddStartListener(Automation* routine);
	void AddAbortListener(Automation* routine);
	void AddContinueListener(Automation* routine);
	vector<Automation*>& GetStartListeners();
	vector<Automation*>& GetAbortListeners();
	vector<Automation*>& GetContinueListeners();
	
	static vector<Event*> event_vector;
	
private:
	vector<Automation*> start_listeners;
    vector<Automation*> abort_listeners;
    vector<Automation*> continue_listeners;
    
};

#endif
