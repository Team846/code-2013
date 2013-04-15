#include "WinchPawl.h"

WinchPawl::WinchPawl()
	: Component("Winch Pawl", DriverStationConfig::DigitalIns::WINCH_PAWL, true),
	  m_configSection("winch_pawl"),
	  Configurable(),
	  m_jaguar(RobotConfig::CAN::WINCH_WORM, "Winch Pawl Jaguar"),
	  m_stopWatch()
{
	m_timedOut = false;
	
	m_jaguar.setCollectionFlags(AsyncCANJaguar::OUTCURR);
	
	m_lastRequestedDutyCycle = 0.0;
	m_lastEnabled = false;
}

WinchPawl::~WinchPawl()
{
	
}

void WinchPawl::enabledPeriodic()
{
	m_lastEnabled = true;
	
	const float requestedDutyCycle = m_winchPawlData->getDutyCycle();
	
	// request to stop the winch pawl
	if(requestedDutyCycle == 0.0)
	{
		m_stopWatch.Stop();
		m_stopWatch.Reset();
		
		m_timedOut = false; // set the timed out flag to false
	}
	// request to start the winch pawl
	else if(m_lastRequestedDutyCycle == 0.0 && requestedDutyCycle != 0.0)
	{
		m_stopWatch.Reset();
		m_stopWatch.Start();
	}
	
	if(m_stopWatch.TotalElapsedSeconds() > m_timeout) // if we've been running too long, disable the motor
	{
		AsyncPrinter::Printf("[SEVERE] WinchPawl::enabledPeriodic(): Timeout!\n");
		
		m_timedOut = true;
		m_stopWatch.Stop();
		m_stopWatch.Reset();
	}
	
	float out = m_timedOut ? 0.0 : requestedDutyCycle;
	m_jaguar.SetDutyCycle(out);
	
	double current = m_jaguar.GetOutputCurrent();
	
	m_winchPawlData->updateMotorCurrent(current);
	
	SmarterDashboard::Instance()->SetTelemetryData<double>(TelemetryType::CLIMBER_WINCH_PAWL_CURRENT, current);
	SmarterDashboard::Instance()->SetTelemetryData<float>(TelemetryType::CLIMBER_WINCH_PAWL_OUTPUT_DUTY_CYCLE, out);
	SmarterDashboard::Instance()->SetTelemetryData<string>(TelemetryType::CLIMBER_WINCH_PAWL_TIMED_OUT, m_timedOut ? "TIMED_OUT" : "OK");
	
	m_lastRequestedDutyCycle = requestedDutyCycle; // keep track of the last requested duty cycle
}

void WinchPawl::disabledPeriodic()
{
	m_jaguar.SetDutyCycle(0.0f);
	
	m_lastEnabled = false;
	
	m_timedOut = false;
	
	m_winchPawlData->updateMotorCurrent(0.0f);
}

void WinchPawl::onEnable()
{
	
}

void WinchPawl::onDisable()
{
	m_jaguar.SetDutyCycle(0.0f);
}

void WinchPawl::Configure()
{
	m_timeout = m_config->Get<double>(m_configSection, "timeout", 5.0);
}
