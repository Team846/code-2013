#ifndef DRIVER_STATION_CONFIG_H_
#define DRIVER_STATION_CONFIG_H_

/*!
 * @brief Contains all constants for driver station configuration
 */

#warning Update driver station config

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
const uint32_t AUTO_AIM = 1;
const uint32_t AUTO_CLIMB = 2;
const uint32_t DISC_TRACK = 3;
const uint32_t APPLY_CONFIG = 5;
const uint32_t LOAD_CONFIG = 6;
const uint32_t SAVE_CONFIG = 7;
const uint32_t RESET_ZERO = 10;
}
namespace DigitalIns
{
const uint32_t MOTORS = 1;
const uint32_t DRIVETRAIN = 2;
const uint32_t COLLECTOR = 3;
const uint32_t SHOOTER = 4;
const uint32_t CLIMBER = 5;
const uint32_t COMP_UNIT_TEST = 8;
}
namespace AnalogIns
{
const uint32_t AUTONOMOUS_DELAY = 1;
const uint32_t AUTONOMOUS_SELECT = 2;
}
}

#endif
