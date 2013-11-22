#ifndef EVENT_H_
#define EVENT_H_

#include <vector>

using namespace std;

class Event
{
public:
	Event();
	virtual ~Event();
	
	virtual bool Fired() = 0;
	
	virtual void Update() = 0;
	
	static void UpdateEvents();
	
private:
	static vector<Event*> event_vector;
};

#endif
