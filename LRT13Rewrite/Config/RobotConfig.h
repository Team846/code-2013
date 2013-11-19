#ifndef ROBOT_CONFIG_H_
#define ROBOT_CONFIG_H_
#include <string>

/*!
 * @brief Contains all constants for robot configuration
 */

#define CHANGEME 99

namespace RobotConfig
{
const uint32_t LOOP_RATE = 50; //hertz
const double ROBOT_WIDTH = 26.1;
const std::string CONFIG_FILE_PATH = "/LRT13.conf";
const std::string PORT_MAPPINGS_FILE_PATH = "/PortMappings.conf";
const std::string ROUTINE_FILE_PATH = "/Autonomous.routine";
const uint32_t MAX_GAME_PIECES = 4;
const double MAX_VOLTAGE = 13;

namespace Drive
{
	const uint32_t THROTTLE_COEFFICIENT = 1;
	const uint32_t THROTTLE_EXPONENT = 1; // must be odd
	const uint32_t BLEND_COEFFICIENT = 1;
	const uint32_t BLEND_EXPONENT = 1;
	const double DEADBAND = 0.03;
}

namespace AutoActions
{
	const uint32_t AUTO_AIM_TIMEOUT = 500; //cycles
}

}

#endif
