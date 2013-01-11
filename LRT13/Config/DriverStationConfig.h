#ifndef DRIVER_STATION_CONFIG_H_
#define DRIVER_STATION_CONFIG_H_

/*!
 * @brief Contains all constants for driver station configuration
 */

namespace DriverStationConfig
{
const static int NUM_JOYSTICK_BUTTONS = 12;//max supported by the DS software. 
										//Having more than 12 can cause a crash in very very very particular scenarios involving broken USB hubs. 
const static int NUM_JOYSTICK_AXES = 6;
const static int DRIVER_STICK_PORT = 1;
const static int OPERATOR_STICK_PORT = 2;
}

#endif
