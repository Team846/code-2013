#ifndef DRIVER_STATION_CONFIG_H_
#define DRIVER_STATION_CONFIG_H_

#include <string>

/*!
 * @brief Contains all constants for driver station configuration.
 */

namespace DriverStationConfig
{
namespace JoystickConfig
{
const uint32_t NUM_JOYSTICK_BUTTONS = 12;// Max supported by the driver station software. 
										// Having more than 12 can cause a crash in very particular scenarios involving broken USB hubs. 
const uint32_t NUM_JOYSTICK_AXES = 6;
const uint32_t NUM_WHEEL_BUTTONS = 11;

const uint32_t NUM_WHEEL_AXES = 3;
const uint32_t DRIVER_STICK_PORT = 1;
const uint32_t OPERATOR_STICK_PORT = 2;
const uint32_t DRIVER_WHEEL_PORT = 3;
}
namespace JoystickButtons
{
//Driver stick
const uint32_t COLLECTOR_SLIDE = 1;
const uint32_t FIRE = 2;
const uint32_t MANUAL_FIRE = 3;
const uint32_t START_CLIMB = 4;

//const uint32_t FORCE_CLIMB_ADVANCE= 4;

const uint32_t VECTOR_DRIVE = 8;
const uint32_t AUTO_AIM_BACKBOARD = 9;
//const uint32_t FEEDER_STATION_APPROACH = 3;
const uint32_t APPLY_CONFIG = 5;
const uint32_t LOAD_CONFIG = 6;
const uint32_t SAVE_CONFIG = 7;
const uint32_t RESET_ZERO = 10;
const uint32_t PAUSE_AUTON = 11;

//Driver wheel
const uint32_t FLASHLIGHT_A = 7;
const uint32_t FLASHLIGHT_B = 11;

//Operator stick
const uint32_t SHOOTER_OFF = 1;
const uint32_t SHOOTER_ON = 2;
const uint32_t ABORT_CLIMB = 2;
const uint32_t CONTINUE_CLIMB = 3;
const uint32_t CHANGE_ANGLE = 4;
const uint32_t COLLECTOR_DOWN_NO_MOTOR = 5;
const uint32_t COLLECT_OPERATOR = 6;
const uint32_t CLIMBER_ARMS = 7;
const uint32_t PAWL_UP = 8;
const uint32_t PAWL_DOWN = 9;
//const uint32_t RECORD = 9;
const uint32_t PURGE = 10;
const uint32_t DISENGAGE_PTO = 11;
const uint32_t ENGAGE_PTO = 12;
const uint32_t STOP_ROBOT = 5;
const uint32_t REVERSE_DRIVE = 6;
}
namespace DigitalIns
{
const uint32_t MOTORS = 1;
const uint32_t DRIVETRAIN = 2;
const uint32_t COLLECTOR = 3;
const uint32_t SHOOTER = 4;
const uint32_t CLIMBER = 5;
const uint32_t WINCH_PAWL = 6;
const uint32_t COMPRESSOR = 7;
const uint32_t COMP_UNIT_TEST = 8;
}
namespace AnalogIns
{
const uint32_t AUTONOMOUS_DELAY = 1;
const uint32_t AUTONOMOUS_SELECT = 2;
const uint32_t NUM_FRISBEES = 3;
}
}

#endif
