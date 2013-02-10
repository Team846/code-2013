#include "Pneumatics.h"
#include "../Config/ConfigManager.h"

Pneumatics* Pneumatics::m_instance = NULL;

#define INIT_PULSED_SOLENOID(x, y) (x).solenoid = (y);\
	(x).counter = (m_pulse_length); \
	(x).state = false; \
	(x).pulsed = true;

Pneumatics::Pneumatics() :
	SynchronizedProcess("Pneumatics"), Configurable(), Loggable(),
			m_name("Pneumatics")
{
	INIT_PULSED_SOLENOID(m_shared, new DoubleSolenoid(
					RobotConfig::Solenoid::COLLECTOR_LEFT_A,
					RobotConfig::Solenoid::COLLECTOR_LEFT_B));

	INIT_PULSED_SOLENOID(m_trajectory, new DoubleSolenoid(
					RobotConfig::Solenoid::COLLECTOR_RIGHT_A,
					RobotConfig::Solenoid::COLLECTOR_RIGHT_B));

	INIT_PULSED_SOLENOID(m_shifter, new DoubleSolenoid(
					RobotConfig::Solenoid::CLIMBER_LEFT_A,
					RobotConfig::Solenoid::CLIMBER_LEFT_B));
	m_shifter.pulsed = false;

	INIT_PULSED_SOLENOID(m_ballcollector, new DoubleSolenoid(
					RobotConfig::Solenoid::CLIMBER_RIGHT_A,
					RobotConfig::Solenoid::CLIMBER_RIGHT_A));

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

void Pneumatics::setShifter(bool on, bool force)
{
	if (on != m_shifter.state || force)
	{
		m_shifter.state = on;
		m_shifter.counter = m_pulse_length;
	}
}

void Pneumatics::setBallCollector(bool on, bool force)
{
	if (on != m_ballcollector.state || force)
	{
		m_ballcollector.state = on;
		m_ballcollector.counter = m_pulse_length;
	}
}

void Pneumatics::setTrajectory(bool on, bool force)
{
	if (on != m_trajectory.state || force)
	{
		m_trajectory.state = on;
		m_trajectory.counter = m_pulse_length;
	}
}

void Pneumatics::setLatch(bool on, bool force)
{
	m_mutex = on;
	on = !on;
	if (on != m_shared.state || force)
	{
		m_shared.state = on;
		m_shared.counter = m_pulse_length;
	}
}

void Pneumatics::setPressurePlate(bool on, bool force)
{
	on = !on;
	if (!m_mutex)
	{
		if (on != m_shared.state || force)
		{
			m_shared.state = on;
			m_shared.counter = m_pulse_length;
		}
	}
}

void Pneumatics::Configure()
{
	ConfigManager* c = ConfigManager::Instance();
	m_pulse_length = c->Get<int> (m_name, "pulseLength", 25);
}

void Pneumatics::Log()
{
	
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
	delete m_shared.solenoid;
	delete m_trajectory.solenoid;
	delete m_shifter.solenoid;
	delete m_ballcollector.solenoid;
	m_compressor->Stop();
	delete m_compressor;
}

INT32 Pneumatics::Tick()
{
	pulse(&m_shifter);
	pulse(&m_ballcollector);
	pulse(&m_trajectory);
	pulse(&m_shared);
	return 0;
}

void Pneumatics::Finalize()
{
	DELETE(m_instance);
}
