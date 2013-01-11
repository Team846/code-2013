#ifndef PAUSE_ACTION_H_
#define PAUSE_ACTION_H_

#include <WPILib.h>

#include "IRobotAction.h"

/*!
 * @brief Sample application of an action.
 */
class PauseAction : public IRobotAction
{
public:
	PauseAction(int millis);
	~PauseAction();
	
	int Run();
	
private:
	int m_millisSleep;
};

#endif
