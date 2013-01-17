#ifndef AUTONOMOUS_H
#define AUTONOMOUS_H

#include "../Process/SynchronizedProcess.h"
#include "Actions/IRobotAction.h"
#include "Actions/PauseAction.h"

namespace data
{
class ComponentData;
}
/*!
 * @brief Performs autonomous routine and contains autonomous functions
 * @author Raphael Chang, Tony Peng
 */
class Autonomous : public SynchronizedProcess
{
public:
	Autonomous(char * taskName, INT32 priority);
	~Autonomous();
protected:
	/*
	 * @brief Runs autonomous routine
	 */
	INT32 Tick();
private:
	IRobotAction* m_autonActions[1];
	
	int m_autonActionCount;

	data::ComponentData* m_data_ptr;
	int m_currentState;
};

#endif
