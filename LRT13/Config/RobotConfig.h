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

namespace Drive
{
	const uint32_t THROTTLE_COEFFICIENT = CHANGEME; // TODO: anurag, change this
	const uint32_t THROTTLE_EXPONENT = 1; // TODO: anurag, change this
	const uint32_t BLEND_COEFFICIENT = CHANGEME; // TODO: anurag, change this
	const uint32_t BLEND_EXPONENT = 3; // TODO: anurag, change this
}

namespace AutoActions
{
	const uint32_t AUTO_AIM_TIMEOUT = 500; //cycles
}

namespace Analog // Ports 1-7
{
}

namespace Digital // Ports 1-14
{
	const uint32_t LEFT_DRIVE_ENCODER_A = 1;
	const uint32_t LEFT_DRIVE_ENCODER_B = 2;
	const uint32_t RIGHT_DRIVE_ENCODER_A = 11;
	const uint32_t RIGHT_DRIVE_ENCODER_B = 12;
	const uint32_t HALL_EFFECT_A = CHANGEME;
	const uint32_t HALL_EFFECT_B = CHANGEME;
	const uint32_t PROXIMITY_A = CHANGEME;
	const uint32_t PROXIMITY_B = CHANGEME;
	const uint32_t STORAGE_SWITCH = CHANGEME;
	const uint32_t PTO_SWITCH_LEFT = CHANGEME;
	const uint32_t PTO_SWITCH_RIGHT = CHANGEME;
	const uint32_t WINCH_GEAR_TOOTH = CHANGEME;
	const uint32_t COMPRESSOR_PRESSURE_SENSOR_PIN = CHANGEME;
}

namespace Solenoid // Ports 1-8
{
	const uint32_t COLLECTOR_A = CHANGEME;
	const uint32_t COLLECTOR_B = CHANGEME;
	const uint32_t CLIMBER_A = CHANGEME;
	const uint32_t CLIMBER_B = CHANGEME;
	
	const uint32_t HOOKS_A  = CHANGEME;
	const uint32_t HOOKS_B = CHANGEME;
	
	const uint32_t STORAGE_EXIT_A = CHANGEME;
	const uint32_t STORAGE_EXIT_B = CHANGEME;
}

namespace Relay // Ports 1-8
{
	const uint32_t COMPRESSOR_RELAY = 1;
}

namespace CAN // Ports 1-16 (2CAN limitation)
{
	const uint32_t LEFT_DRIVE_A = 1;
	const uint32_t LEFT_DRIVE_B = 4;
	const uint32_t RIGHT_DRIVE_A = 2;
	const uint32_t RIGHT_DRIVE_B = 3;
	const uint32_t COLLECTOR = CHANGEME;
	const uint32_t SHOOTER_A = CHANGEME;
	const uint32_t SHOOTER_B = CHANGEME;
	const uint32_t WINCH_WORM = CHANGEME;
}

namespace PWM
{
	const uint32_t LEFT_PTO_SERVO = 1;
	const uint32_t RIGHT_PTO_SERVO = 2;
}

}

#endif
