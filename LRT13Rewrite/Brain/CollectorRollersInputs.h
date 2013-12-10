#ifndef COLLECTOR_ROLLERS_INPUTS_H_
#define COLLECTOR_ROLLERS_INPUTS_H_

#include "InputProcessor.h"
#include "../ComponentData/CollectorRollersData.h"

/*!
 * @brief Process joystick information for controlling the collector rollers.
 */
class CollectorRollersInputs : public InputProcessor
{
public:
	CollectorRollersInputs();
	
	void Update();
	
private:
	DebouncedJoystick *m_driver_stick;
	DebouncedJoystick *m_operator_stick;
	
	CollectorRollersData *collectorRollersData;
};

#endif
