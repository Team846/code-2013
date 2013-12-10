#include <stdio.h>
#include "sysLib.h"
#include "AsyncCANJaguar.h"
#include "../Utils/AsyncPrinter.h"

#define DISABLE_SETPOINT_CACHING 0

vector<AsyncCANJaguar*> AsyncCANJaguar::jaguar_vector;

AsyncCANJaguar::AsyncCANJaguar(UINT8 channel, std::string name) :
	SynchronizedProcess((std::string("AsyncCANJaguar #") + Util::ToString<int>(channel)).c_str(),Task::kDefaultPriority - 2),
	CANJaguar(channel),
	LRTSpeedController(("AsyncCANJaguar" + name).c_str())
{
	m_task_name = "JAG#" + Util::ToString<int>(channel);
	m_channel = channel;
	m_setpoint.setValue(0.0);
	m_should_disable_control = false;
	m_should_disable_position_limits = false;
	m_collection_flags = 0;
	m_last_game_mode = RobotState::Instance().GameMode();
	jaguar_vector.push_back(this);

	printf("Constructed AsyncCANJaguar %s on channel %d\n", name.c_str(), channel);
}

AsyncCANJaguar::~AsyncCANJaguar()
{
}

void AsyncCANJaguar::Send()
{
	RunOneCycle();
}

void AsyncCANJaguar::Tick()
{
	if (m_channel != 0)
	{
		// Set data
		// If game mode has changed, uncache all cached values
		if (m_last_game_mode != RobotState::Instance().GameMode())
		{
			ResetCache();
		}

		// Modes and setpoints should be automatically refreshed
		m_neutral_mode.incrementCounter();
		m_setpoint.incrementCounter();

		// If any of the modes or setpoints have changed, resend all of them
		if (m_neutral_mode.hasNewValue() || m_control_mode.hasNewValue()
				|| m_setpoint.hasNewValue())
		{
			m_neutral_mode.uncache();
			m_control_mode.uncache();
			m_setpoint.uncache();
		}

#if DISABLE_SETPOINT_CACHING
		ResetCache();
#endif

		if (m_control_mode.hasNewValue())
		{
			ResetCache();
			CANJaguar::ChangeControlMode(m_control_mode.getValue());
		}

		if (m_position_reference.hasNewValue())
		{
			CANJaguar::SetPositionReference(m_position_reference.getValue());
		}

		if (m_speed_reference.hasNewValue())
		{
			CANJaguar::SetSpeedReference(m_speed_reference.getValue());
		}

		if (m_pid_p.hasNewValue() || m_pid_i.hasNewValue()
				|| m_pid_d.hasNewValue())
		{
			CANJaguar::SetPID(m_pid_p.getValue(), m_pid_i.getValue(),
					m_pid_d.getValue());
		}

		if (m_voltage_ramp_rate.hasNewValue())
		{
			CANJaguar::SetVoltageRampRate(m_voltage_ramp_rate.getValue());
		}

		if (m_max_output_voltage.hasNewValue())
		{
			CANJaguar::ConfigMaxOutputVoltage(m_max_output_voltage.getValue());
		}

		if (m_fault_time.hasNewValue())
		{
			CANJaguar::ConfigFaultTime(m_fault_time.getValue());
		}

		if (m_expiration.hasNewValue())
		{
			CANJaguar::SetExpiration(m_expiration.getValue());
		}

		if (m_encoder_codes_per_rev.hasNewValue())
		{
			CANJaguar::ConfigEncoderCodesPerRev(
					m_encoder_codes_per_rev.getValue());
		}

		if (m_potentiometer_turns.hasNewValue())
		{
			CANJaguar::ConfigPotentiometerTurns(
					m_potentiometer_turns.getValue());
		}

		if (m_should_disable_position_limits)
		{
			CANJaguar::DisableSoftPositionLimits();
			m_should_disable_position_limits = false;
		}

		if (m_forward_limit_position.hasNewValue()
				|| m_reverse_limit_position.hasNewValue())
		{
			CANJaguar::ConfigSoftPositionLimits(
					m_forward_limit_position.getValue(),
					m_reverse_limit_position.getValue());
		}

		if (m_enable_control.hasNewValue())
		{
			CANJaguar::EnableControl(m_enable_control.getValue());
		}

		if (m_should_disable_control)
		{
			CANJaguar::DisableControl();
			m_should_disable_control = false;
		}

		// Change the mode, then do the setpoint.
		if (m_neutral_mode.hasNewValue())
		{
			CANJaguar::ConfigNeutralMode((CANJaguar::NeutralMode)m_neutral_mode.getValue());
		}

		if (m_setpoint.hasNewValue())
		{
			CANJaguar::Set(m_setpoint.getValue());
		}

		// Get data	
		if (m_collection_flags & OUTVOLT)
		{
			float v = CANJaguar::GetOutputVoltage();
			if (StatusOK())
				m_output_voltage = v;
			else
				Println("Invalid output voltage; not storing\n");
		}

		if (m_collection_flags & OUTCURR)
		{
			float current = CANJaguar::GetOutputCurrent();
			if (StatusOK())
				m_output_current = current;
			else
				Println("Invalid current; not storing\n");
		}

		if (m_collection_flags & POS)
		{
			float pos = CANJaguar::GetPosition();
			if (StatusOK())
				m_position = pos;
			else
				Println("Invalid position value; not storing\n");
		}

		if (m_collection_flags & PID)
		{
			float p = CANJaguar::GetP();
			if (StatusOK())
				m_p = p;
			else
				Println("Invalid proportional gain; not storing\n");
			float i = CANJaguar::GetI();
			if (StatusOK())
				m_i = i;
			else
				Println("Invalid integral gain; not storing\n");
			float d = CANJaguar::GetD();
			if (StatusOK())
				m_d = d;
			else
				Println("Invalid derivative gain; not storing\n");
		}

		if (m_collection_flags & SPEEDREF)
		{
			SpeedReference s = CANJaguar::GetSpeedReference();
			if (StatusOK())
				m_speed_ref = s;
			else
				Println("Invalid speed reference; not storing\n");
		}

		if (m_collection_flags & POSREF)
		{
			PositionReference p = CANJaguar::GetPositionReference();
			if (StatusOK())
				m_position_ref = p;
			else
				Println("Invalid position reference; not storing");
		}

		if (m_collection_flags & CTRLMODE)
		{
			ControlMode c = CANJaguar::GetControlMode();
			if (StatusOK())
				m_ctrl_mode = c;
			else
				Println("Invalid control mode; not storing\n");
		}

		if (m_collection_flags & BUSVOLT)
		{
			float v = CANJaguar::GetBusVoltage();
			if (StatusOK())
				m_bus_voltage = v;
			else
				Println("Invalid bus voltage; not storing\n");
		}

		if (m_collection_flags & TEMP)
		{
			float t = CANJaguar::GetTemperature();
			if (StatusOK())
				m_temperature = t;
			else
				Println("Invalid temperature; not storing\n");
		}

		if (m_collection_flags & SPEED)
		{
			float s = CANJaguar::GetSpeed();
			if (StatusOK())
				m_speed = s;
			else
				Println("Invalid speed; not storing\n");
		}

		if (m_collection_flags & FWDLIMOK)
		{
			bool b = CANJaguar::GetForwardLimitOK();
			if (StatusOK())
				m_fwd_limit_ok = b;
			else
				Println("Invalid forward limit status, not storing\n");
		}

		if (m_collection_flags & REVLIMOK)
		{
			bool b = CANJaguar::GetReverseLimitOK();
			if (StatusOK())
				m_rev_limit_ok = b;
			else
				Println("Invalid reverse limit status, not storing\n");
		}

		if (m_collection_flags & PWRCYCLE)
		{
			bool b = CANJaguar::GetPowerCycled();
			if (StatusOK())
				m_pwr_cyc = b;
			else
				Println("Invalid power cycle status, not storing\n");
		}

		if (m_collection_flags & EXPIRE)
		{
			float e = CANJaguar::GetExpiration();
			if (StatusOK())
				m_expire = e;
			else
				Println("Invalid expiration time, not storing\n");
		}

		if (m_collection_flags & VALUE)
		{
			float v = CANJaguar::Get();
			if (StatusOK())
				m_value = v;
			else
				Println("Invalid hardware value, not storing\n");
		}

		m_last_game_mode = RobotState::Instance().GameMode();
	}
	else
	{
		AsyncPrinter::Printf("[ERROR] AsyncCANJaguar: %s on channel 0.\n", GetName());
	}
}

int AsyncCANJaguar::GetChannel()
{
	return m_channel;
}

void AsyncCANJaguar::SetDutyCycle(float duty_cycle)
{
	m_control_mode.setValue(kPercentVbus);
	m_setpoint.setValue(duty_cycle);
}

void AsyncCANJaguar::SetPosition(float position)
{
	m_control_mode.setValue(kPosition);
	m_setpoint.setValue(position);
}

void AsyncCANJaguar::SetVelocity(float velocity)
{
	m_control_mode.setValue(kSpeed);
	m_setpoint.setValue(velocity);
}

void AsyncCANJaguar::Set(float setpoint, UINT8 syncGroup)
{
	printf("[WARNING] Calling Set() in AsyncCANJaguar: %s, use SetDutyCycle() instead.\n;", GetName());
	m_setpoint.setValue(setpoint);
}

void AsyncCANJaguar::SetPositionReference(CANJaguar::PositionReference reference)
{
	m_position_reference.setValue(reference);
}

void AsyncCANJaguar::SetSpeedReference(CANJaguar::SpeedReference reference)
{
	m_speed_reference.setValue(reference);
}

void AsyncCANJaguar::SetPID(double p, double i, double d)
{
	m_pid_p.setValue(p);
	m_pid_i.setValue(i);
	m_pid_d.setValue(d);
}

void AsyncCANJaguar::ConfigNeutralMode(LRTSpeedController::NeutralMode mode)
{
	m_neutral_mode.setValue(mode);
}

void AsyncCANJaguar::ChangeControlMode(ControlMode mode)
{
	m_control_mode.setValue(mode);
}

void AsyncCANJaguar::EnableControl(double encoderInitialPosition)
{
	m_enable_control.setValue(encoderInitialPosition);
}

void AsyncCANJaguar::DisableControl()
{
	m_should_disable_control = true;
}

void AsyncCANJaguar::SetVoltageRampRate(double rampRate)
{
	m_voltage_ramp_rate.setValue(rampRate);
}

void AsyncCANJaguar::ConfigEncoderCodesPerRev(UINT16 codesPerRev)
{
	m_encoder_codes_per_rev.setValue(codesPerRev);
}

void AsyncCANJaguar::ConfigPotentiometerTurns(UINT16 turns)
{
	m_potentiometer_turns.setValue(turns);
}

void AsyncCANJaguar::ConfigSoftPositionLimits(double forwardLimitPosition,
		double reverseLimitPosition)
{
	m_forward_limit_position.setValue(forwardLimitPosition);
	m_reverse_limit_position.setValue(reverseLimitPosition);
}

void AsyncCANJaguar::DisableSoftPositionLimits()
{
	m_should_disable_position_limits = true;
}

void AsyncCANJaguar::ConfigMaxOutputVoltage(double voltage)
{
	m_max_output_voltage.setValue(voltage);
}

void AsyncCANJaguar::ConfigFaultTime(float faultTime)
{
	m_fault_time.setValue(faultTime);
}

void AsyncCANJaguar::SetExpiration(float timeout)
{
	m_expiration.setValue(timeout);
}

void AsyncCANJaguar::ResetCache()
{
	m_control_mode.uncache();
	m_setpoint.uncache();
	m_neutral_mode.uncache();
	m_pid_p.uncache();
	m_pid_i.uncache();
	m_pid_d.uncache();
	m_position_reference.uncache();
	m_speed_reference.uncache();
	m_enable_control.uncache();
	m_voltage_ramp_rate.uncache();
	m_fault_time.uncache();
	m_encoder_codes_per_rev.uncache();
	m_potentiometer_turns.uncache();
	m_max_output_voltage.uncache();
}

bool AsyncCANJaguar::StatusOK()
{
	return GetFaults() == 0;
}

void AsyncCANJaguar::RemoveCollectionFlags(uint32_t flags)
{
	m_collection_flags &= ~(flags);
}

void AsyncCANJaguar::AddCollectionFlags(uint32_t flags)
{
	m_collection_flags |= flags;
}

void AsyncCANJaguar::SetCollectionFlags(uint32_t flags)
{
	m_collection_flags = flags;
}

float AsyncCANJaguar::GetDutyCycle()
{
	return m_setpoint.peek();
}

float AsyncCANJaguar::GetHardwareValue()
{
	return m_value;
}

LRTSpeedController::NeutralMode AsyncCANJaguar::GetNeutralMode()
{
	return m_neutral_mode.peek();
}

double AsyncCANJaguar::GetP()
{
	return m_p;
}

double AsyncCANJaguar::GetI()
{
	return m_i;
}

double AsyncCANJaguar::GetD()
{
	return m_d;
}

CANJaguar::SpeedReference AsyncCANJaguar::GetSpeedReference()
{
	return m_speed_ref;
}

CANJaguar::PositionReference AsyncCANJaguar::GetPositionReference()
{
	return m_position_ref;
}
CANJaguar::ControlMode AsyncCANJaguar::GetControlMode()
{
	return m_ctrl_mode;
}

float AsyncCANJaguar::GetBusVoltage()
{
	return m_bus_voltage;
}

float AsyncCANJaguar::GetOutputVoltage()
{
	return m_output_voltage;
}

double AsyncCANJaguar::GetOutputCurrent()
{
	return m_output_current;
}

float AsyncCANJaguar::GetTemperature()
{
	return m_temperature;
}

float AsyncCANJaguar::GetPosition()
{
	return m_position;
}

double AsyncCANJaguar::GetSpeed()
{
	return m_speed;
}

bool AsyncCANJaguar::GetForwardLimitOK()
{
	return m_fwd_limit_ok;
}

bool AsyncCANJaguar::GetReverseLimitOK()
{
	return m_rev_limit_ok;
}

bool AsyncCANJaguar::GetPowerCycled()
{
	return m_pwr_cyc;
}

float AsyncCANJaguar::GetExpiration()
{
	return m_expire;
}

void AsyncCANJaguar::Println(const char * str)
{
	AsyncPrinter::Printf("%s: %s", GetName(), str);
}
