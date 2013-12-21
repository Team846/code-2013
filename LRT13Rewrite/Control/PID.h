#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

#include "RunningSum.h"
/*!
 * @brief Simple implementation of a PID controller
 * @author Robert Ying
 * @author Brian Axelrod
 */

class PID
{
public:
	/*!
	 * @brief Constructs a PID controller. All gains are in constant (not timebase) form.
	 * @param p_gain proportional gain
	 * @param i_gain integral gain
	 * @param d_gain derivative gain
	 * @param ff_gain feedforward gain (defaults to 1.0)
	 * @param i_decay integral decay (defaults to 0.5)
	 * @param feedforward whether or not feedforward is used
	 * @param filterFreq the frequency of the low pass filter
	 */
	PID(double p_gain, double i_gain, double d_gain, double ff_gain = 1.0,
			double i_decay = 0.5, bool feedforward = true, double filterFreq = 7.0);

	/*!
	 * @brief Constructs a PID controller with no gains set
	 */
	PID();

	/*!
	 * @brief Sets PID parameters. All gains are in constant (not timebase) form
	 * @brief This method does reset accumulated error and differential error
	 * @param p_gain proportional gain
	 * @param i_gain integral gain
	 * @param d_gain derivative gain
	 * @param ff_gain feedforward gain (defaults to 1.0)
	 * @param i_decay integral decay (defaults to 0.5)
	 * @param feedforward whether or not feedforward is used
	 * @param filterFreq the frequency of the low pass filter
	 */
	void SetParameters(double p_gain, double i_gain, double d_gain,
					double ff_gain = 1.0, double i_decay = 0.87,
					bool feedforward = true, double filterFreq = 7.0);

	/*!
	 * @brief Updates the PID controller, call on each loop.
	 * @param dt time differential
	 * @return pid output
	 */
	virtual double Update(double dt);

	/*!
	 * @brief Sets the feedback input.
	 * @param input PID input
	 */
	void SetInput(double input);

	/*!
	 * @brief Sets the setpoint.
	 * @param setpoint the setpoint to set
	 */
	virtual void SetSetpoint(double setpoint);

	/*!
	 * @brief Gets the PID output.
	 * @return PID output
	 */
	double GetOutput();

	/*!
	 * @brief Gets the p gain.
	 * @return proportional PID gain
	 */
	double GetProportionalGain();

	/*!
	 * @brief Gets the i gain.
	 * @return integral PID gain
	 */
	double GetIntegralGain();

	/*!
	 * @brief Gets the d gain.
	 * @return derivative PID gain
	 */
	double GetDerivativeGain();

	/*!
	 * @brief Gets the feed forward gain.
	 * @return feed forward gain
	 */
	double GetFeedForwardGain();

	/*!
	 * @brief Gets the integral decay rate.
	 * @return integral decay rate
	 */
	double GetIntegralDecay();

	/*!
	 * @brief Gets the input value.
	 * @return input
	 */
	double GetInput();

	/*!
	 * @brief Gets the setpoint value.
	 * @return setpoint
	 */
	double GetSetpoint();

	/*!
	 * @brief Gets the error value.
	 * @return error
	 */
	double GetError();

	/*!
	 * @brief Gets the accumulated running sum.
	 * @return running sum
	 */
	double GetAccumulatedError();

	/*!
	 * @brief Gets the previous error (one iteration ago).
	 * @return previous error
	 */
	double GetPreviousError();

	/*!
	 * @brief m_status flag for feed-forward PID.
	 * @return whether this controller uses feedforward PID
	 */
	bool IsFeedForward();

	/*!
	 * @brief Resets PID setpoint, input, error, accumulated error, differential, and output.
	 */
	void Reset();

	/*!
	 * @brief Disables PID (output returns setpoint).
	 */
	void DisablePID();

	/*!
	 * @brief Enables PID.
	 */
	void EnablePID();
	
	/*!
	 * @brief Enables an IIR filter on output.
	 */
	void SetIIREnabled(bool enabled);
	
	/*!
	 * @brief Sets the IFR Decay rate.
	 */
	void SetIIRDecay(double decay);
	
private:
	double m_proportional_gain;
	double m_integral_gain;
	double m_derivative_gain;
	double m_feedforward_gain;
	double m_integral_decay;
	double m_input;
	double m_output;
	double m_setpoint;
	double m_error;
	double m_prev_error;
	double m_acc_error;
	bool m_is_feed_forward;
	bool m_enabled;
	
	bool m_IIREnabled;
	RunningSum m_runningSum;
};
#endif
