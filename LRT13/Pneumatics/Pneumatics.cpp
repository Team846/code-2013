#include "Pneumatics.h"

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

	INIT_PULSED_SOLENOID(m_climber, new DoubleSolenoid(
					RobotConfig::Solenoid::CLIMBER_A,
					RobotConfig::Solenoid::CLIMBER_B));
	
	INIT_PULSED_SOLENOID(m_hook, new DoubleSolenoid(
					RobotConfig::Solenoid::HOOKS_A,
					RobotConfig::Solenoid::HOOKS_B));

	INIT_PULSED_SOLENOID(m_storageExit, new DoubleSolenoid(
					2,
					RobotConfig::Solenoid::STORAGE_EXIT_A,
					RobotConfig::Solenoid::STORAGE_EXIT_B));
	
	INIT_PULSED_SOLENOID(m_shooterAngler, new DoubleSolenoid(
					RobotConfig::Solenoid::SHOOTER_ANGLER_A,
					RobotConfig::Solenoid::SHOOTER_ANGLER_B));

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

void Pneumatics::setClimberArm(bool on, bool force)
{
	if (on != m_climber.state || force)
	{
		m_climber.state = on;
		m_climber.counter = m_pulse_length;
	}
}

void Pneumatics::setShooterAngler(bool on, bool force)
{
	if(on != m_shooterAngler.state || force)
	{
		m_shooterAngler.state = on;
		m_climber.counter = m_pulse_length;
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

void Pneumatics::setHookPosition(bool on, bool force)
{
	if (on != m_hook.state || force)
	{
		m_hook.state = on;
		m_hook.counter = m_pulse_length;
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
	DELETE(m_collector.solenoid);
	DELETE(m_hook.solenoid);
	DELETE(m_climber.solenoid);
	DELETE(m_shooterAngler.solenoid);
	m_compressor->Stop();
	DELETE(m_compressor);
}

INT32 Pneumatics::Tick()
{
	pulse(&m_collector);
	pulse(&m_hook);
	pulse(&m_climber);
	pulse(&m_storageExit);
	pulse(&m_shooterAngler);
//	AsyncPrinter::Printf("Compressor: %s\n", (m_compressor->Enabled() ? "on" : "off"));
//	AsyncPrinter::Printf("Pressure switch: %d\n", m_compressor->GetPressureSwitchValue());
	
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

bool Pneumatics::GetStorageExitState()
{
	return m_storageExit.state;
}

bool Pneumatics::GetClimberState()
{
	return m_climber.state;
}

bool Pneumatics::GetShooterAngleState()
{
	return m_shooterAngler.state;
}

bool Pneumatics::GetHookState()
{
	return m_hook.state;
}
