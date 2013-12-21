#ifndef ANGLE_H_
#define ANGLE_H_

#include "Automation.h"
#include "../../Actuators/Pneumatics.h"

class Angle : public Automation
{
public:
	Angle(bool high);

	Status Start(Event *trigger);
	bool Run();
	Status Abort(Event *trigger);
	void AllocateResources();
	
private:
	Pneumatics *m_angle;
	bool m_high;
};

#endif
