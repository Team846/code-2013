#include "Pneumatics.h"

#include <sstream>

vector<Pneumatics*> Pneumatics::pneumatic_vector;
Compressor *Pneumatics::m_compressor;

Pneumatics::Pneumatics(uint32_t forward, uint32_t reverse, const char *name) :
	Actuator(name),
	Configurable("Pneumatics"),
	Loggable("Pneumatics" + std::string(name)),
	m_configSection("Pneumatics")
{
	printf("Created DoubleSolenoid %s\n", name);
	solenoid = new DoubleSolenoid(forward, reverse);
	counter = 0;
	pulsed = true;
	state = OFF;
	
	pneumatic_vector.push_back(this);
}

Pneumatics::Pneumatics(uint32_t forward, uint32_t reverse, uint8_t module, const char *name) :
	Actuator(name),
	Configurable("Pneumatics"),
	Loggable("Pneumatics" + std::string(name)),
	m_configSection("Pneumatics")
{
	printf("Created DoubleSolenoid %s\n", name);
	solenoid = new DoubleSolenoid(module, forward, reverse);
	counter = 0;
	pulsed = true;
	state = OFF;

	pneumatic_vector.push_back(this);
}

Pneumatics::Pneumatics(uint32_t forward, const char *name) :
	Actuator(name),
	Configurable("Pneumatics"),
	Loggable("Pneumatics" + std::string(name)),
	m_configSection("Pneumatics")
{
	printf("Created Solenoid %s\n", name);
	solenoid = new Solenoid(forward);
	counter = 0;
	pulsed = false;
	state = OFF;

	pneumatic_vector.push_back(this);
}

Pneumatics::Pneumatics(uint32_t forward, uint8_t module, const char *name) :
	Actuator(name),
	Configurable("Pneumatics"),
	Loggable("Pneumatics" + std::string(name)),
	m_configSection("Pneumatics")
{
	printf("Created Solenoid %s\n", name);
	solenoid = new Solenoid(module, forward);
	counter = 0;
	pulsed = false;
	state = OFF;

	pneumatic_vector.push_back(this);
}

Pneumatics::~Pneumatics()
{
	delete solenoid;
}

void Pneumatics::Update()
{
	if (pulsed && dynamic_cast<DoubleSolenoid*>(solenoid))
	{
		if (counter > 0)
		{
			counter--;
			if (state == FORWARD)
			{
				dynamic_cast<DoubleSolenoid*>(solenoid)->Set(DoubleSolenoid::kForward);
			}
			else if (state == REVERSE)
			{
				dynamic_cast<DoubleSolenoid*>(solenoid)->Set(DoubleSolenoid::kReverse);
			}
			else if (state == OFF)
			{
				dynamic_cast<DoubleSolenoid*>(solenoid)->Set(DoubleSolenoid::kOff);
			}
		}
		else
		{
			counter = 0;
			dynamic_cast<DoubleSolenoid*>(solenoid)->Set(DoubleSolenoid::kOff);
		}
	}
	else
	{
		if (state == FORWARD)
		{
			if (dynamic_cast<Solenoid*>(solenoid))
				dynamic_cast<Solenoid*>(solenoid)->Set(true);
			else if (dynamic_cast<DoubleSolenoid*>(solenoid))
				dynamic_cast<DoubleSolenoid*>(solenoid)->Set(DoubleSolenoid::kForward);
		}
		else if (state == OFF)
		{
			if (dynamic_cast<Solenoid*>(solenoid))
				dynamic_cast<Solenoid*>(solenoid)->Set(false);
			else if (dynamic_cast<DoubleSolenoid*>(solenoid))
				dynamic_cast<DoubleSolenoid*>(solenoid)->Set(DoubleSolenoid::kOff);
		}
		else if (state == REVERSE)
		{
			if (dynamic_cast<DoubleSolenoid*>(solenoid))
				dynamic_cast<DoubleSolenoid*>(solenoid)->Set(DoubleSolenoid::kReverse);
		}
	}
}

void Pneumatics::CreateCompressor()
{
	m_compressor = new Compressor(
			ConfigPortMappings::Get("Digital/COMPRESSOR_PRESSURE_SENSOR_PIN"),
			ConfigPortMappings::Get("Relay/COMPRESSOR_RELAY"));
	m_compressor->Start();
}

void Pneumatics::DestroyCompressor()
{
	m_compressor->Stop();
	delete m_compressor;
}

void Pneumatics::SetCompressor(bool on)
{
	if (on)
	{
		m_compressor->Start();
	}
	else
	{
		m_compressor->Stop();
	}
}

void Pneumatics::Set(State on, bool force)
{
	if (on != state || force)
	{
		state = on;
		if (dynamic_cast<Solenoid*>(solenoid) && state == REVERSE)
		{
			state = OFF;
		}
		if (pulsed)
		{
			counter = m_pulse_length;
		}
	}
}

Pneumatics::State Pneumatics::Get()
{
	return state;
}

Pneumatics::State Pneumatics::GetHardwareValue()
{
	State current;
	if (dynamic_cast<DoubleSolenoid*>(solenoid))
	{
		if (dynamic_cast<DoubleSolenoid*>(solenoid)->Get() == DoubleSolenoid::kForward)
			current = FORWARD;
		else if (dynamic_cast<DoubleSolenoid*>(solenoid)->Get() == DoubleSolenoid::kReverse)
			current = REVERSE;
		else
			current = OFF;
	}
	else if (dynamic_cast<Solenoid*>(solenoid))
	{
		if (dynamic_cast<Solenoid*>(solenoid)->Get())
			current = FORWARD;
		else
			current = OFF;
	}
	return current;
}

void Pneumatics::Configure()
{
	ConfigRuntime *c = ConfigRuntime::Instance();
	m_pulse_length = c->Get<int> (m_configSection, "pulseLength", 25);
}

void Pneumatics::Log()
{
	LogToFile(&state, "State");
}
