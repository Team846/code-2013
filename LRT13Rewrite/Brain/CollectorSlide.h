#ifndef COLLECTOR_SLIDE_H_
#define COLLECTOR_SLIDE_H_

#include <string>
#include "../Logging/Loggable.h"
#include "Automation.h"
#include "../Actuators/Pneumatics.h"

/*!
 * @brief Slides the collector up and down while changing angle.
 */
class CollectorSlide : public Automation, public Configurable
{
public:
	typedef enum State
	{
		BEGIN = 0,
		LINE_UP = 1,
		HOOKS_DOWN = 2,
		ARMS_UP = 3,
		ARMS_DOWN = 4,
		CLIMB_PREPARE = 5,
		HOOKS_UP = 6,
		CLIMB = 7
	};
	
	CollectorSlide();
	~CollectorSlide();
	
	void Configure();
	void Log();
	
protected:
	Status Start(Event *trigger);
	bool Run();
	Status Abort(Event *trigger);
	void AllocateResources();
	
private:
	Pneumatics *m_angle;
	Pneumatics *m_collector;
	int m_extendCycles;
	int m_retractCycles;
	int m_timer;
	bool aborting;
	bool done;
};

#endif
