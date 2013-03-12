#include "InputUnitTest.h"

InputUnitTest::InputUnitTest() :
	UnitTest()
{
	m_stick = new DebouncedJoystick(DriverStationConfig::JoystickConfig::DRIVER_STICK_PORT,
									DriverStationConfig::JoystickConfig::NUM_JOYSTICK_BUTTONS,
									DriverStationConfig::JoystickConfig::NUM_JOYSTICK_AXES);
	
	m_wheel = new DebouncedJoystick(DriverStationConfig::JoystickConfig::DRIVER_WHEEL_PORT,
									DriverStationConfig::JoystickConfig::NUM_WHEEL_BUTTONS,
									DriverStationConfig::JoystickConfig::NUM_WHEEL_AXES);
}

InputUnitTest::~InputUnitTest()
{
	delete m_stick;
	m_stick = NULL;
	
	delete m_wheel;
	m_wheel = NULL;
}

void InputUnitTest::Run()
{
	Pneumatics::Instance()->Start();
	
	while(true)
	{
		if (!IsDisabled())
		{
			m_stick->Update();

			Pneumatics::Instance()->setCompressor(true);
			if(m_stick->IsButtonDown(DriverStationConfig::JoystickButtons::APPLY_CONFIG))
			{
				printf("apply config is down.\n");
				Pneumatics::Instance()->setClimberArm(true);
			}
			else
			{
				Pneumatics::Instance()->setClimberArm(false);
			}
			
			if(m_stick->IsButtonDown(DriverStationConfig::JoystickButtons::AUTO_AIM_BACKBOARD))
			{
				printf("auto aim is down.\n");
				Pneumatics::Instance()->setCollector(true);
			}
			else
			{
				Pneumatics::Instance()->setCollector(false);
			}
			
			if(m_stick->IsButtonDown(DriverStationConfig::JoystickButtons::FEEDER_STATION_APPROACH))
			{
				printf("disc track is down.\n");
				Pneumatics::Instance()->setHookPosition(true);
			}
			else
			{
				Pneumatics::Instance()->setHookPosition(false);
			}
			
			if(m_stick->IsButtonDown(DriverStationConfig::JoystickButtons::LOAD_CONFIG))
			{
				printf("load config is down.\n");
				Pneumatics::Instance()->setShooterAngler(true);
			}
			else
			{
				Pneumatics::Instance()->setShooterAngler(false);
			}
			
			Pneumatics::Instance()->RunOneCycle();
			
			if(m_stick->IsButtonDown(DriverStationConfig::JoystickButtons::RESET_ZERO))
			{
				printf("reset zero is down.\n");
				Pneumatics::Instance()->setStorageExit(true);
			}
			else
			{
				Pneumatics::Instance()->setStorageExit(false);
			}
			
			if(m_stick->IsButtonDown(DriverStationConfig::JoystickButtons::SAVE_CONFIG))
			{
				printf("save config is down.\n");
			}
			
			float rawZAxis = m_stick->GetAxis(Joystick::kZAxis);
			float rawYAxis = m_stick->GetAxis(Joystick::kYAxis);
			float rawXAxis = m_wheel->GetAxis(Joystick::kXAxis);
	//		
	//		printf("Stick Raw Z Axis: %f. \n", rawZAxis);
	//		printf("Stick Raw Y Axis: %f. \n", rawYAxis);
	//		printf("Wheel Raw X Axis: %f. \n", rawXAxis);
		}
		else
		{
			Pneumatics::Instance()->setCompressor(false);
		}
		taskDelay(sysClkRateGet() / 10);
	}
}
