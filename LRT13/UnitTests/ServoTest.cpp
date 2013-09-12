#include "ServoTest.h"
#include <vxWorks.h>
#include <sysLib.h>

ServoTest::ServoTest()
: leftServoValue(1000), rightServoValue(1000)
{
	leftServo = new Servo(RobotConfig::Servo::LEFT_PTO_SERVO);
	rightServo = new Servo(RobotConfig::Servo::RIGHT_PTO_SERVO);
	m_stick = new DebouncedJoystick(DriverStationConfig::JoystickConfig::DRIVER_STICK_PORT,
									DriverStationConfig::JoystickConfig::NUM_JOYSTICK_BUTTONS,
									DriverStationConfig::JoystickConfig::NUM_JOYSTICK_AXES);
}

void ServoTest::Run()
{
	const int MIN_VAL = 727;
	const int MAX_VAL = 2252;
	while(true)
	{
		m_stick->Update();
		if(m_stick->IsButtonClicked(DriverStationConfig::JoystickButtons::APPLY_CONFIG))
		{
			leftServoValue -= 10;
		}
		if(m_stick->IsButtonClicked(DriverStationConfig::JoystickButtons::LOAD_CONFIG))
		{
			leftServoValue += 10;
		}
		if(m_stick->IsButtonClicked(12))
		{
			rightServoValue -= 10;
		}
		if(m_stick->IsButtonClicked(11))
		{
			rightServoValue += 10;
		}
		printf("Left Servo Setpoint: %f\n", leftServoValue);
		printf("Right Servo Setpoint: %f\n", rightServoValue);
		printf("Left Servo Value: %f\n", leftServo->Get());
		printf("Right Servo Value: %f\n", rightServo->Get());
		float ms = Util::Clamp<float>(leftServoValue, MIN_VAL, MAX_VAL);
		float val = (float)(ms - MIN_VAL) / (MAX_VAL - MIN_VAL);
		leftServo->Set(val);
		ms = Util::Clamp<float>(rightServoValue, MIN_VAL, MAX_VAL);
		val = (float)(ms - MIN_VAL) / (MAX_VAL - MIN_VAL);
		rightServo->Set(val);
		taskDelay(sysClkRateGet() / 4);
	}
}
