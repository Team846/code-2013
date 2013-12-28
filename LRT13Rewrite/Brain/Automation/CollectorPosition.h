#ifndef COLLECTOR_POSITION_H_
#define COLLECTOR_POSITION_H_

#include "Automation.h"
#include "../../Actuators/Pneumatics.h"

class CollectorPosition : public Automation
{
public:
	CollectorPosition(bool down);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
	
private:
	Pneumatics *m_collector;
	bool m_down;
};

#endif
