#ifndef LRT_TALON_H_
#define LRT_TALON_H_

#include "Talon.h"
#include "LRTSpeedController.h"
#include "../Logging/Loggable.h"

#include "DigitalOutput.h"
#include <vector>

using namespace std;

/*!
 * @brief Wrapper for Talon speed controller.
 */
class LRTTalon : public Talon, public LRTSpeedController, public Loggable
{
public:
	explicit LRTTalon(UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	LRTTalon(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	virtual ~LRTTalon();
	virtual void SetDutyCycle(float speed);
	virtual float GetDutyCycle();
	virtual float GetHardwareValue();
	void Set(float speed);
	virtual float Get();
	virtual void Disable();
	virtual void PIDWrite(float output);
	
	virtual void ConfigNeutralMode(LRTSpeedController::NeutralMode mode);
	
	/*!
	 * @brief Writes the values to the Talon.
	 */
	void Send();
	
	void Log();
	
	static vector<LRTTalon*> talon_vector;
	
private:
	float m_pwm;
	
	DigitalOutput* m_brake_jumper;
	LRTSpeedController::NeutralMode m_neutral;
};

#endif /* LRT_TALON_H_ */
