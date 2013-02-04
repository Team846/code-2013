#ifndef TELEOPERATED_INPUTS_H
#define TELEOPERATED_INPUTS_H

#include <WPILib.h>

#include "../Process/SynchronizedProcess.h"
#include "../ComponentData/RobotData.h"
#include "../ComponentData/DrivetrainData.h"
#include "../ComponentData/ConfigLoaderData.h"

#include "../Config/ConfigManager.h"

using namespace data;
using namespace data::drivetrain;

namespace data
{
class ComponentData;
}
class DebouncedJoystick;
class AutoActions;

/*!
 * @brief Processes driver station inputs in teleoperated mode
 * @author Raphael Chang
 */
class TeleopInputs : public SynchronizedProcess
{
public:
	TeleopInputs(char * taskName, INT32 priority=Task::kDefaultPriority);
	~TeleopInputs();
protected:
	/*
	 * @brief Processes joystick inputs and sets component data
	 */
	INT32 Tick();
private:
	void Update();
	
	data::ComponentData* m_componentData;
	DebouncedJoystick* m_driver_stick;
	DebouncedJoystick* m_operator_stick;
	DebouncedJoystick* m_driver_wheel;
	
	AutoActions* m_autoActions;
};

#endif
