#ifndef SERVO_TEST_H_
#define SERVO_TEST_H_

#include "../UnitTests/UnitTest.h"
#include "Servo.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../Operations/DebouncedJoystick.h"
#include "../Utils/Util.h"

class ServoTest : public UnitTest
{
public:
	ServoTest();
	void Run();
private:
	Servo* leftServo;
	Servo* rightServo;
	float leftServoValue;
	float rightServoValue;
	DebouncedJoystick* m_stick;
};

#endif
