#include "LinearFilter.h"

LinearFilter* LinearFilter::SinglePoleIIRLowPassFilter(double decay)
{
	vector<double> ffGains(1, 1.0 - decay);
	vector<double> fbGains(1, -decay); // Negative because past values are added (subtracted in formula)
	return new LinearFilter(ffGains, fbGains);
}

LinearFilter* LinearFilter::SinglePoleIIRHighPassFilter(double decay)
{
	vector<double> ffGains(2);
	ffGains[0] = (1 + decay) / 2;
	ffGains[1] = -(1 + decay) / 2;
	vector<double> fbGains(1, -decay); // Negative because past values are added (subtracted in formula)
	return new LinearFilter(ffGains, fbGains);
}

LinearFilter* LinearFilter::MovingAverageFilter(int samples)
{
	vector<double> ffGains(samples, 1.0 / samples);
	return new LinearFilter(ffGains, vector<double>());
}

LinearFilter* LinearFilter::PIDController(double Kp, double Ki, double Kd)
{
	vector<double> ffGains(3);
	ffGains[0] = Kp + Ki + Kd;
	ffGains[1] = -Kp - 2 * Kd;
	ffGains[2] = Kd;
	vector<double> fbGains(1, -1);
	return new LinearFilter(ffGains, fbGains);
}

LinearFilter::LinearFilter(vector<double> ffGains, vector<double> fbGains)
{
	m_ffGains = ffGains;
	m_fbGains = fbGains;
	m_inputSample.resize(ffGains.size(), 0);
	m_outputSample.resize(fbGains.size(), 0);
}

double LinearFilter::Update(double input)
{
	float output = 0.0;
	
	m_inputSample.pop_back(); // Remove oldest input value from sample
	m_inputSample.push_front(input); // Add new input value to sample
	
	// Calculate the new output using the formula y[n] = (b0*x[n] + b1*x[n-1] + ... + bN*x[n-N]) - (a0*y[n-1] + a2*y[n-2] + ... + aM*y[n-M])
	for (unsigned int i = 0; i < m_ffGains.size(); i++) // Add input values multiplied by gains
	{
		output += m_inputSample[i] * m_ffGains[i];
	}
	for (unsigned int i = 0; i < m_fbGains.size(); i++) // Subtract output values multiplied by gains
	{
		output -= m_outputSample[i] * m_fbGains[i];
	}
	
	m_outputSample.pop_back(); // Remove oldest output value from sample
	m_outputSample.push_front(output); // Add new output to sample
	
	return output;
}

void LinearFilter::Reset()
{
	m_inputSample.clear();
	m_outputSample.clear();
	m_inputSample.resize(m_ffGains.size());
	m_outputSample.resize(m_fbGains.size());
}
