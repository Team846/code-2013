#ifndef AUTOMATION_H_
#define AUTOMATION_H_

#include <vector>
#include "Events/Event.h"

using namespace std;

class Automation
{
public:
	Automation();
	virtual ~Automation();
	
	void Update();
	
	void AddStartEvent(Event *trigger);
	void AddAbortEvent(Event *trigger);
	
protected:
	virtual void Start(Event *trigger) = 0;
	virtual void Run() = 0;
	virtual void Abort(Event *trigger) = 0;
	
private:
	static vector<Automation*> running;
	vector<Event*> start_events;
	vector<Event*> abort_events;
	bool m_running;
};

#endif
