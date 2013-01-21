#ifndef ROBOT_CONFIG_H_
#define ROBOT_CONFIG_H_

/*!
 * @brief Contains all constants for robot configuration
 */

#warning Update robot config

#define CHANGEME 0

namespace RobotConfig
{
const uint32_t LOOP_RATE = 50; //hertz
const string CONFIG_FILE_PATH = "/LRT13.conf";

#warning conflicts with DriverStationConfig::DigitalIO
namespace digital_io
{
	const uint32_t LEFT_DRIVE_ENCODER_A = CHANGEME;
	const uint32_t LEFT_DRIVE_ENCODER_B = CHANGEME;
	const uint32_t RIGHT_DRIVE_ENCODER_A = CHANGEME;
	const uint32_t RIGHT_DRIVE_ENCODER_B = CHANGEME;
}

namespace can
{
	const uint32_t LEFT_DRIVE_A = CHANGEME;
	const uint32_t LEFT_DRIVE_B = CHANGEME;
	const uint32_t RIGHT_DRIVE_A = CHANGEME;
	const uint32_t RIGHT_DRIVE_B = CHANGEME;
	const uint32_t COLLECTOR = CHANGEME;
	const uint32_t SHOOTER = CHANGEME;
}

}

#endif
