#include "PID.h"
#include "../Utils/AsyncPrinter.h"
#define IIR_DECAY(FREQ) (2 * 3.14159 * (FREQ) / 50)

PID::PID(double p_gain, double i_gain, double d_gain, double ff_gain,
		double i_decay, bool feedforward, double filterFreq) :
		m_runningSum(IIR_DECAY(filterFreq))
{
	SetParameters(p_gain, i_gain, d_gain, ff_gain, i_decay, feedforward, filterFreq);
	m_IIREnabled = false;
}

PID::PID() :
		m_runningSum(IIR_DECAY(7.0))
{
	SetParameters(0, 0, 0);
	m_IIREnabled = false;
}

void PID::SetParameters(double p_gain, double i_gain, double d_gain,
		double ff_gain, double i_decay, bool feedforward, double filterFreq)
{
	Reset();
	m_proportional_gain = p_gain;
	m_integral_gain = i_gain;
	m_derivative_gain = d_gain;
	m_feedforward_gain = ff_gain;
	m_integral_decay = i_decay != 1.0 ? i_decay : 0.9999999999999;
	m_is_feed_forward = feedforward;
	m_runningSum.setDecayConstant(IIR_DECAY(filterFreq));
	EnablePID();
}

double PID::Update(double dt)
{
	m_error = m_setpoint - m_input;
	//	AsyncPrinter::Printf("Error: %.02f\r\n", m_error);

	if (m_IIREnabled)
	{
		m_error = m_runningSum.UpdateSum(m_error);
	}
	else
	{
		m_runningSum.Clear();
	}

	// calculate discrete derivative
	double delta = (m_error - m_prev_error) / dt;

	// approximate with riemann sum and decay
	m_acc_error *= m_integral_decay;
	m_acc_error += m_error * dt;
	if (m_acc_error != m_acc_error) // catch NaN
	{
		m_acc_error = 0;
	}
	double integral = m_acc_error / (1 - m_integral_decay);

	// magic PID line
	double PID_output = m_proportional_gain * (m_error + m_integral_gain
			* integral + m_derivative_gain * delta);

	if (m_is_feed_forward)
	{
		// feed-forward PID
		m_output = m_setpoint * m_feedforward_gain + PID_output;
	}
	else
	{
		// standard PID
		m_output = PID_output;
	}

	m_prev_error = m_error;

	return m_output;
}

void PID::SetSetpoint(double setpoint)
{
	m_setpoint = setpoint;
}

void PID::SetInput(double input)
{
	m_input = input;
}

double PID::GetProportionalGain()
{
	return m_proportional_gain;
}

double PID::GetIntegralGain()
{
	return m_integral_gain;
}

double PID::GetDerivativeGain()
{
	return m_derivative_gain;
}

double PID::GetFeedForwardGain()
{
	return m_feedforward_gain;
}

double PID::GetIntegralDecay()
{
	return m_integral_decay;
}

double PID::GetInput()
{
	return m_input;
}

double PID::GetSetpoint()
{
	return m_setpoint;
}

double PID::GetError()
{
	return m_error;
}

double PID::GetAccumulatedError()
{
	return m_acc_error;
}

double PID::GetPreviousError()
{
	return m_prev_error;
}

double PID::GetOutput()
{
	if (m_enabled)
		return m_output;
	else
		return m_setpoint;
}

void PID::DisablePID()
{
	m_enabled = false;
}

void PID::EnablePID()
{
	m_enabled = true;
}

void PID::Reset()
{
	m_acc_error = 0.0;
	m_error = 0.0;
	m_prev_error = 0.0;
	m_input = 0.0;
	m_output = 0.0;
	m_setpoint = 0.0;
}

void PID::SetIIREnabled(bool enabled)
{
	m_IIREnabled = enabled;
	if (!enabled)
		m_runningSum.Clear();
}

void PID::SetIIRDecay(double decay)
{
	m_runningSum.setDecayConstant(decay);
}
