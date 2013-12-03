#ifndef CLIMB_H_
#define CLIMB_H_

#include <string>

#include "../Config/Configurable.h"
#include "../Logging/Loggable.h"
#include "../Utils/LCD.h"
#include "AnalogChannel.h"

#include "Automation.h"

/*!
 * @brief Climb routine.
 */
class Climb : public Automation, public Configurable, public Loggable
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
	
	Climb();
	~Climb();
	
	void Configure();
	void Log();
	
protected:
	Status Start(Event *trigger);
	bool Run();
	Status Abort(Event *trigger);
	void AllocateResources();
	
private:
	AnalogChannel *m_pressure_a;
	AnalogChannel *m_pressure_b;
	NetworkTable *table;
	State m_state;
	State m_previousState;
	string m_stateString;
	int m_timer;
	int m_swingWaitTicks;
	double m_pressure_scale;
};

#endif
