#ifndef TELEOPERATED_INPUTS_H
#define TELEOPERATED_INPUTS_H

#include <WPILib.h>

#include "../Process/AsyncProcess.h"
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

/*!
 * @brief Processes driver station inputs in teleoperated mode
 * @author Raphael Chang
 */
class TeleopInputs : public AsyncProcess
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
	
	data::ComponentData* m_actionData;
	DebouncedJoystick* m_driver_stick;
	DebouncedJoystick* m_operator_stick;
	DebouncedJoystick* m_driver_wheel;
};

#endif
