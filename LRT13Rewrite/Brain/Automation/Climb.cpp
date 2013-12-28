#include "Climb.h"
#include "../../Config/DriverStationConfig.h"
#include "../../Config/ConfigPortMappings.h"
#include "../../Config/ConfigRuntime.h"
#include "../../Utils/AsyncPrinter.h"
#include "../Events/JoystickPressedEvent.h"

//#define STEP

Climb::Climb() :
	Automation("Climb"),
	Configurable("Climb"),
	Loggable("Climb")
{
	table = NetworkTable::GetTable("RobotData");
	m_pressure_a = new AnalogChannel(ConfigPortMappings::Get("Analog/PRESSURE_A"));
	m_pressure_b = new AnalogChannel(ConfigPortMappings::Get("Analog/PRESSURE_B"));
	m_state = BEGIN;
	m_previousState = BEGIN;
	m_timer = 0;
	m_stateString = "NOTHING";
}

Climb::~Climb()
{

}

bool Climb::Start()
{
	m_state = BEGIN;
	m_previousState = BEGIN;
	return true;
}

void Climb::AllocateResources()
{
	
}

bool Climb::Run()
{
	m_stateString = "NOTHING";

	switch(m_state)
	{
	case BEGIN:
		m_stateString = "BEGIN";
		
		break;
	case LINE_UP:
		m_stateString = "LINE_UP";

		// Line up to the first bar
		
		if(Continued())
		{
			if (dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())
					&& dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())->GetJoystick()->GetPort() == DriverStationConfig::JoystickConfig::DRIVER_STICK_PORT)
				m_state = HOOKS_DOWN;
		}
		break;
	case HOOKS_DOWN:
		m_stateString = "HOOKS_DOWN";

		if(Continued())
		{
			if (dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())
					&& dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())->GetJoystick()->GetPort() == DriverStationConfig::JoystickConfig::OPERATOR_STICK_PORT)
				m_state = ARMS_UP;
		}
		break;
	case ARMS_UP:
		m_stateString = "ARMS_UP";
		
		if(Continued())
		{
			if (dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())
					&& dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())->GetJoystick()->GetPort() == DriverStationConfig::JoystickConfig::OPERATOR_STICK_PORT)
				m_state = ARMS_DOWN;
		}
		break;
	case ARMS_DOWN:
		m_stateString = "ARMS_DOWN";

		if(Continued())
		{
			if (dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())
					&& dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())->GetJoystick()->GetPort() == DriverStationConfig::JoystickConfig::OPERATOR_STICK_PORT)
				m_state = CLIMB_PREPARE;
		}
		break;
	case CLIMB_PREPARE:
		m_stateString = "CLIMB_PREPARE";
		
		m_timer = 0;
		
#ifdef STEP
		if(Continued())
		{
			if (dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())
					&& dynamic_cast<JoystickPressedEvent*>(GetContinueEvent())->GetJoystick()->GetPort() == DriverStationConfig::JoystickConfig::OPERATOR_STICK_PORT)
				m_state = HOOKS_UP;
		}
#endif
		m_state = HOOKS_UP;
		
		break;
	case HOOKS_UP:
		m_stateString = "HOOKS_UP";
		
		m_timer++;
		if(m_timer >= m_swingWaitTicks)
		{
			m_state = CLIMB;
		}
		break;
	case CLIMB:
		m_stateString = "CLIMB";
		
		return true;
		break;
	}
	
	static int printCounter = 0;
	
	if(printCounter++ % 20 == 0 || m_state != m_previousState)
		AsyncPrinter::Printf("Climb state: %s\n", m_stateString.c_str());
	LCD::Instance()->Print(4, 0, true, "Climb: %s", m_stateString.c_str());
	LCD::Instance()->Print(4, 0, true, "P: %.4f", (m_pressure_b->GetAverageVoltage() - m_pressure_a->GetAverageVoltage()) * m_pressure_scale);
	table->PutNumber("Pressure", (m_pressure_b->GetAverageVoltage() - m_pressure_a->GetAverageVoltage()) * m_pressure_scale);
	m_previousState = m_state;
	return false;
}

bool Climb::Abort()
{
	m_state = BEGIN;
	m_previousState = BEGIN;
	return true;
}

void Climb::Configure()
{
	m_swingWaitTicks = m_config->Get<int>(m_configSection, "swingWaitTicks", 50);
	m_pressure_scale = m_config->Get<double> (m_configSection, "pressure_scale", 1400);
}

void Climb::Log()
{
	LogToFile(&m_state, "State");
}
