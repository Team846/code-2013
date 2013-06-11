#ifndef ROBOT_CONFIG_H_
#define ROBOT_CONFIG_H_
#include <string>

/*!
 * @brief Contains all constants for robot configuration
 */

#warning Update robot config

#define CHANGEME 99

namespace RobotConfig
{
const uint32_t LOOP_RATE = 50; //hertz
const std::string CONFIG_FILE_PATH = "/LRT13.conf";
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

namespace Analog // Ports 1-7
{
}

namespace Digital // Ports 1-14
{
	const uint32_t LEFT_DRIVE_ENCODER_A = 1;
	const uint32_t LEFT_DRIVE_ENCODER_B = 2;
	const uint32_t WINCH_GEAR_TOOTH = 3;
	const uint32_t PROXIMITY_SHOOTER = 4;
	const uint32_t PROXIMITY_COLLECTOR = 5;
	const uint32_t HALL_EFFECT_A = 6;
	const uint32_t HALL_EFFECT_B = 7;
	const uint32_t PTO_SWITCH_LEFT = 9;
	const uint32_t PTO_SWITCH_RIGHT = 10;
	const uint32_t RIGHT_DRIVE_ENCODER_A = 11;
	const uint32_t RIGHT_DRIVE_ENCODER_B = 12;
//	const uint32_t LED_DATA_OUT = 13;
	const uint32_t FLASHLIGHT = 13; // Flashlight change
	const uint32_t COMPRESSOR_PRESSURE_SENSOR_PIN = 14;
	const uint32_t TWOCAN_MOSFET = CHANGEME;
}

namespace Solenoid // Ports 1-8
{
	const uint32_t COLLECTOR_A = 1;
	const uint32_t COLLECTOR_B = 2;
	const uint32_t CLIMBER_A = 3;
	const uint32_t CLIMBER_B = 4;
	const uint32_t HOOKS_A  = 5;
	const uint32_t HOOKS_B = 6;
	const uint32_t STORAGE_EXIT_A = 1; // Module 4
	const uint32_t STORAGE_EXIT_B = 2; // Module 4
	const uint32_t SHOOTER_ANGLER_A = 7;
	const uint32_t SHOOTER_ANGLER_B = 8;
}

namespace Relay // Ports 1-8
{
	const uint32_t COMPRESSOR_RELAY = 1;
	const uint32_t FLASHLIGHT = 2;
}

namespace CAN // Ports 1-16 (2CAN limitation)
{
	const uint32_t LEFT_DRIVE_A = 1;
	const uint32_t LEFT_DRIVE_B = 14; //4
	const uint32_t RIGHT_DRIVE_A = 2;
	const uint32_t RIGHT_DRIVE_B = 3;
	const uint32_t COLLECTOR = 10; //8//13 on practice bot
	const uint32_t SHOOTER_A = 7;
	const uint32_t SHOOTER_B = 5;
	const uint32_t WINCH_WORM = 6;
}

namespace Servo
{
	const uint32_t LEFT_PTO_SERVO = 1;
	const uint32_t RIGHT_PTO_SERVO = 2;
}


}

#endif
