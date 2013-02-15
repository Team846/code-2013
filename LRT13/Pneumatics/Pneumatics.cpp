#include "Pneumatics.h"
#include "../Config/ConfigManager.h"
#include "../Utils/AsyncPrinter.h"

Pneumatics* Pneumatics::m_instance = NULL;

#define INIT_PULSED_SOLENOID(x, y) (x).solenoid = (y);\
	(x).counter = (m_pulse_length); \
	(x).state = false; \
	(x).pulsed = true;

Pneumatics::Pneumatics() :
	SynchronizedProcess("Pneumatics"), Configurable(), Loggable(),
			m_configSection("Pneumatics")
{
	INIT_PULSED_SOLENOID(m_collector, new DoubleSolenoid(
					RobotConfig::Solenoid::COLLECTOR_A,
					RobotConfig::Solenoid::COLLECTOR_B));

	INIT_PULSED_SOLENOID(m_climberLowerInner, new DoubleSolenoid(
					RobotConfig::Solenoid::CLIMBER_LOWER_INNER_A,
					RobotConfig::Solenoid::CLIMBER_LOWER_INNER_B));

	INIT_PULSED_SOLENOID(m_climberLowerOuter, new DoubleSolenoid(
					RobotConfig::Solenoid::CLIMBER_LOWER_OUTER_A,
					RobotConfig::Solenoid::CLIMBER_LOWER_OUTER_B));

	INIT_PULSED_SOLENOID(m_climberMiddle, new DoubleSolenoid(
					RobotConfig::Solenoid::CLIMBER_MIDDLE_A,
					RobotConfig::Solenoid::CLIMBER_MIDDLE_B));

	INIT_PULSED_SOLENOID(m_climberUpper, new DoubleSolenoid(
					RobotConfig::Solenoid::CLIMBER_UPPER_A,
					RobotConfig::Solenoid::CLIMBER_UPPER_B));

	INIT_PULSED_SOLENOID(m_storageExit, new DoubleSolenoid(
					RobotConfig::Solenoid::STORAGE_EXIT,
					RobotConfig::Solenoid::STORAGE_EXIT));

	m_compressor = new Compressor(
			RobotConfig::Digital::COMPRESSOR_PRESSURE_SENSOR_PIN,
			RobotConfig::Relay::COMPRESSOR_RELAY);
	m_compressor->Start();
}

#undef INIT_PULSED_SOLENOID

Pneumatics* Pneumatics::Instance()
{
	if (m_instance == NULL)
	{
		m_instance = new Pneumatics();
	}
	return m_instance;
}

void Pneumatics::setClimberLowerInner(bool on, bool force)
{
	if (on != m_climberLowerInner.state || force)
	{
		m_climberLowerInner.state = on;
		m_climberLowerInner.counter = m_pulse_length;
	}
}

void Pneumatics::setClimberLowerOuter(bool on, bool force)
{
	if (on != m_climberLowerOuter.state || force)
	{
		m_climberLowerOuter.state = on;
		m_climberLowerOuter.counter = m_pulse_length;
	}
}

void Pneumatics::setClimberMiddle(bool on, bool force)
{
	if (on != m_climberMiddle.state || force)
	{
		m_climberMiddle.state = on;
		m_climberMiddle.counter = m_pulse_length;
	}
}

void Pneumatics::setClimberUpper(bool on, bool force)
{
	if (on != m_climberUpper.state || force)
	{
		m_climberUpper.state = on;
		m_climberUpper.counter = m_pulse_length;
	}
}

void Pneumatics::setCollector(bool on, bool force)
{
	if (on != m_collector.state || force)
	{
		m_collector.state = on;
		m_collector.counter = m_pulse_length;
	}
}

void Pneumatics::setStorageExit(bool on, bool force)
{
	if (on != m_storageExit.state || force)
	{
		m_storageExit.state = on;
		m_storageExit.counter = m_pulse_length;
	}
}

void Pneumatics::pulse(PulsedSolenoid * ptr)
{
	if (ptr->pulsed)
	{
		if (ptr->counter > 0)
		{
			ptr->counter = ptr->counter - 1;
			if (ptr->state)
			{
				ptr->solenoid->Set(DoubleSolenoid::kForward);
			}
			else
			{
				ptr->solenoid->Set(DoubleSolenoid::kReverse);
			}
		}
		else
		{
			ptr->counter = 0;
			ptr->solenoid->Set(DoubleSolenoid::kOff);
		}
	}
	else
	{
		if (ptr->state)
		{
			ptr->solenoid->Set(DoubleSolenoid::kForward);
		}
		else
		{
			ptr->solenoid->Set(DoubleSolenoid::kReverse);
		}
	}
}

void Pneumatics::setCompressor(bool on)
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

Pneumatics::~Pneumatics()
{
	delete m_collector.solenoid;
	delete m_climberLowerInner.solenoid;
	delete m_climberLowerOuter.solenoid;
	delete m_climberMiddle.solenoid;
	m_compressor->Stop();
	delete m_compressor;
}

INT32 Pneumatics::Tick()
{
	pulse(&m_collector);
	pulse(&m_climberLowerInner);
	pulse(&m_climberLowerOuter);
	pulse(&m_climberMiddle);
	pulse(&m_climberUpper);
	pulse(&m_storageExit);
	return 0;
}

void Pneumatics::Finalize()
{
	DELETE(m_instance);
}

void Pneumatics::Configure()
{
	ConfigManager* c = ConfigManager::Instance();
	m_pulse_length = c->Get<int> (m_configSection, "pulseLength", 25);
}

void Pneumatics::Log()
{
	
}

void Pneumatics::Configure()
{
	ConfigManager* c = ConfigManager::Instance();
	m_pulse_length = c->Get<int> (m_configSection, "pulseLength", 25);
}

void Pneumatics::Log()
{
	
}
