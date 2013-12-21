#ifndef COLLECTOR_POSITION_H_
#define COLLECTOR_POSITION_H_

#include "Automation.h"
#include "../../Actuators/Pneumatics.h"

class CollectorPosition : public Automation
{
public:
	CollectorPosition(bool down);

	Status Start(Event *trigger);
	bool Run();
	Status Abort(Event *trigger);
	void AllocateResources();
	
private:
	Pneumatics *m_collector;
	bool m_down;
};

#endif
