#ifndef LINEAR_FILTER_H_
#define LINEAR_FILTER_H_

#include <vector>
#include <deque>

using namespace std;

/*!
 * @brief A recursive linear FIR or IIR filter.
 * Filters are of the form: y[n] = (b0*x[n] + b1*x[n-1] + ... + bN*x[n-N]) - (a0*y[n-1] + a2*y[n-2] + ... + aM*y[n-M])
 * y[n] is the sample of the output signal at time n
 * x[n] is the sample of the input signal at time n
 * y[n-1] is the output from the last time step
 * x[n-1] is the input from the last time step
 * b0 to bN are the feedforward gains
 * a0 to aM are the feedback gains
 */
class LinearFilter
{
public:
	/*!
	 * @brief Creates a linear filter.
	 * @param ffGains the feedforward gains
	 * @param fbGains the feedback gains
	 */
	LinearFilter(vector<double> ffGains, vector<double> fbGains);
	
	/*!
	 * @brief Creates a single pole IIR low-pass filter.
	 * @param decay the gain of the filter, between 0 and 1 (decay constant)
	 * @return the created filter
	 */
	static LinearFilter* SinglePoleIIRLowPassFilter(double decay);
	/*!
	 * @brief Creates a single pole IIR high-pass filter.
	 * @param decay the gain of the filter, between 0 and 1 (decay constant)
	 * @return the created filter
	 */
	static LinearFilter* SinglePoleIIRHighPassFilter(double decay);
	/*!
	 * @brief Creates a moving average FIR filter.
	 * @param samples the number of samples to average
	 * @return the created filter
	 */
	static LinearFilter* MovingAverageFilter(int samples);
	/*!
	 * @brief Creates a PID controller filter of the form y[n] = y[n-1] + (Kp+Ki+Kd)*x[n] + (-Kp-2*Kd)*x[n-1] + Kd*x[n-2].
	 * @param Kp the proportional gain
	 * @param Ki the integral gain
	 * @param Kd the derivative gain
	 * @return the created filter
	 */
	static LinearFilter* PIDController(double Kp, double Ki, double Kd);
	
	/*!
	 * @brief Calculates the next value of the filter output. Must be called periodically.
	 * @param input the next value of the input signal (error if PID filter)
	 * @return the next value of the output signal
	 */
	double Update(double input);
	
	/*!
	 * @brief Resets the values of the filter.
	 */
	void Reset();
	
private:
	deque<double> m_inputSample;
	deque<double> m_outputSample;
	vector<double> m_ffGains;
	vector<double> m_fbGains;
};

#endif
