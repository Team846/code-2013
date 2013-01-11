#include "TeleopInputs.h"
#include "DebouncedJoystick.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/DriverStationConfig.h"

using namespace data;

TeleopInputs::TeleopInputs(char * taskName, INT32 priority) :
	AsyncProcess(taskName, priority)
{
	m_data_ptr = ComponentData::GetInstance();
	m_driver = new DebouncedJoystick(DriverStationConfig::DRIVER_STICK_PORT,
			DriverStationConfig::NUM_JOYSTICK_BUTTONS,
			DriverStationConfig::NUM_JOYSTICK_AXES);
	m_operator = new DebouncedJoystick(DriverStationConfig::OPERATOR_STICK_PORT,
			DriverStationConfig::NUM_JOYSTICK_BUTTONS,
			DriverStationConfig::NUM_JOYSTICK_AXES);
}

TeleopInputs::~TeleopInputs()
{

}

INT32 TeleopInputs::Tick()
{
	m_driver->Update();
	m_operator->Update();
	
	Update();
	
	return 0;
}

void TeleopInputs::Update()
{
	RobotData::RobotState current_state = RobotData::getCurrentState();
	
	if(current_state == RobotData::TELEOP)
	{
		if(m_driver->IsButtonDown(RESET_ZERO))
		{
			
		}
		else
		{
			
		}
	}
	
	if(m_driver->IsButtonJustPressed(SAVE_CONFIG))
		ConfigManager::Instance()->Save();
	if(m_driver->IsButtonJustPressed(LOAD_CONFIG))
		ConfigManager::Instance()->Load();
	if(m_driver->IsButtonJustPressed(APPLY_CONFIG))
		ConfigManager::Instance()->ConfigureAll();
}
