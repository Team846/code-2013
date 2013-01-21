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
const static int NUM_JOYSTICK_BUTTONS = 12;//max supported by the DS software. 
										//Having more than 12 can cause a crash in very very very particular scenarios involving broken USB hubs. 
const static int NUM_JOYSTICK_AXES = 6;
const static int NUM_WHEEL_BUTTONS = 11;

const static int NUM_WHEEL_AXES = 3;
const static int DRIVER_STICK_PORT = 1;
const static int OPERATOR_STICK_PORT = 2;
const static int DRIVER_WHEEL_PORT = 3;
}
namespace JoystickButtons
{
const static int APPLY_CONFIG = 5;
const static int LOAD_CONFIG = 6;
const static int SAVE_CONFIG = 7;
const static int RESET_ZERO = 10;
}
namespace DigitalIO
{
const uint32_t MOTORS = 1;
const uint32_t DRIVETRAIN = 2;
const uint32_t COLLECTOR = 3;
const uint32_t SHOOTER = 4;
}
}

#endif
