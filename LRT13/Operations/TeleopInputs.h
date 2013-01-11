#ifndef TELEOPERATED_INPUTS_H
#define TELEOPERATED_INPUTS_H

#include <WPILib.h>

#include "../Process/AsyncProcess.h"
#include "../ComponentData/RobotData.h"
#include "../ComponentData/DrivetrainData.h"

#include "../Config/ConfigManager.h"

#define APPLY_CONFIG 5
#define LOAD_CONFIG 6
#define SAVE_CONFIG 7
#define RESET_ZERO 10

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
	
	data::ComponentData* m_data_ptr;
	DebouncedJoystick* m_driver;
	DebouncedJoystick* m_operator;
};

#endif
