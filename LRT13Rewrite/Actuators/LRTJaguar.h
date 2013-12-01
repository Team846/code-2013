#ifndef LRT_JAGUAR_H_
#define LRT_JAGUAR_H_

#include "Jaguar.h"
#include "LRTSpeedController.h"
#include "../Logging/Loggable.h"

#include "DigitalOutput.h"
#include <vector>

using namespace std;

/*!
 * @brief Wrapper for PWM Jaguars.
 */
class LRTJaguar : public Jaguar, public LRTSpeedController, public Loggable
{
public:
	
	explicit LRTJaguar(UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	LRTJaguar(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	virtual ~LRTJaguar();
	virtual void SetDutyCycle(float value);
	virtual float GetDutyCycle();
	virtual float GetHardwareValue();
	void Set(float speed);
	virtual float Get();
	virtual void Disable();
	virtual void PIDWrite(float output);
	
	virtual void ConfigNeutralMode(LRTSpeedController::NeutralMode mode);
	
	/*!
	 * Writes the values to the Jaguar.
	 */
	void Send();
	
	void Log();
	
	static vector<LRTJaguar*> jaguar_vector;
	
private:
	float m_pwm;
	
	DigitalOutput* m_brake_jumper;
	LRTSpeedController::NeutralMode m_neutral;
};

#endif /* LRT_TALON_H_ */
