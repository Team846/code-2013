#include "Pneumatics.h"

#include <sstream>

vector<Pneumatics*> Pneumatics::pneumatic_vector;
Compressor *Pneumatics::m_compressor;

Pneumatics::Pneumatics(uint32_t forward, uint32_t reverse, const char *name) :
	SynchronizedProcess((std::string("Pneumatics") + std::string(name)).c_str(), Task::kDefaultPriority - 1),
	Configurable(),
	m_configSection("Pneumatics")
{
	printf("Created DoubleSolenoid %s\n", name);
	solenoid = new DoubleSolenoid(forward, reverse);
	counter = 0;
	pulsed = true;
	m_name = name;
	state = OFF;
	
	pneumatic_vector.push_back(this);
}

Pneumatics::Pneumatics(uint32_t forward, uint32_t reverse, uint8_t module, const char *name) :
	SynchronizedProcess("Pneumatics", Task::kDefaultPriority - 1),
	Configurable(),
	m_configSection("Pneumatics")
{
	printf("Created DoubleSolenoid %s\n", name);
	solenoid = new DoubleSolenoid(module, forward, reverse);
	counter = 0;
	pulsed = true;
	m_name = name;
	state = OFF;

	pneumatic_vector.push_back(this);
}

Pneumatics::Pneumatics(uint32_t forward, const char *name) :
	SynchronizedProcess((std::string("Pneumatics") + std::string(name)).c_str(), Task::kDefaultPriority - 1),
	Configurable(),
	m_configSection("Pneumatics")
{
	printf("Created Solenoid %s\n", name);
	solenoid = new Solenoid(forward);
	counter = 0;
	pulsed = false;
	m_name = name;
	state = OFF;

	pneumatic_vector.push_back(this);
}

Pneumatics::Pneumatics(uint32_t forward, uint8_t module, const char *name) :
	SynchronizedProcess("Pneumatics", Task::kDefaultPriority - 1),
	Configurable(),
	m_configSection("Pneumatics")
{
	printf("Created Solenoid %s\n", name);
	solenoid = new Solenoid(module, forward);
	counter = 0;
	pulsed = false;
	m_name = name;
	state = OFF;

	pneumatic_vector.push_back(this);
}

Pneumatics::~Pneumatics()
{
	delete solenoid;
}

void Pneumatics::Update()
{
	RunOneCycle();
}

void Pneumatics::CreateCompressor()
{
	m_compressor = new Compressor(
			RobotConfig::Digital::COMPRESSOR_PRESSURE_SENSOR_PIN,
			RobotConfig::Relay::COMPRESSOR_RELAY);
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

void Pneumatics::Set(bool on, bool force)
{
	if (on != state || force)
	{
		if (dynamic_cast<DoubleSolenoid*>(solenoid))
		{
			if (on)
				state = FORWARD;
			else
				state = REVERSE;
		}
		else if (dynamic_cast<Solenoid*>(solenoid))
		{
			if (on)
				state = FORWARD;
			else
				state = OFF;
		}
		counter = m_pulse_length;
	}
}

bool Pneumatics::Get()
{
	return state;
}

INT32 Pneumatics::Tick()
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
	
	return 0;
}

void Pneumatics::Configure()
{
	ConfigManager *c = ConfigManager::Instance();
	m_pulse_length = c->Get<int> (m_configSection, "pulseLength", 25);
}

const char* Pneumatics::GetName()
{
	return m_name;
}
