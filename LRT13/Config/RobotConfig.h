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

namespace drive
{
	const uint32_t THROTTLE_COEFFICIENT = CHANGEME; // TODO: anurag, change this
	const uint32_t THROTTLE_EXPONENT = 1; // TODO: anurag, change this
	const uint32_t BLEND_COEFFICIENT = CHANGEME; // TODO: anurag, change this
	const uint32_t BLEND_EXPONENT = 3; // TODO: anurag, change this
}

namespace analog // Ports 1-7
{

}

namespace digital // Ports 1-14
{
	const uint32_t RIGHT_DRIVE_ENCODER_A = 1;
	const uint32_t RIGHT_DRIVE_ENCODER_B = 2;
	const uint32_t LEFT_DRIVE_ENCODER_A = 4;
	const uint32_t LEFT_DRIVE_ENCODER_B = 3;
	const uint32_t HALL_EFFECT = CHANGEME;
}

namespace can // Ports 1-16 (2CAN limitation)
{
	const uint32_t LEFT_DRIVE_A = 2;
	const uint32_t LEFT_DRIVE_B = 3;
	const uint32_t RIGHT_DRIVE_A = 4;
	const uint32_t RIGHT_DRIVE_B = 5;
	const uint32_t COLLECTOR = CHANGEME;
	const uint32_t SHOOTER = CHANGEME;
}

}

#endif
