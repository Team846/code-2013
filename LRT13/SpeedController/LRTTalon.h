#ifndef LRT_TALON_H_
#define LRT_TALON_H_

#include "Talon.h"
#include "LRTSpeedController.h"

#include "DigitalOutput.h"
#include <vector>

using namespace std;

class LRTTalon : public Talon, public LRTSpeedController
{
public:
	explicit LRTTalon(UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	LRTTalon(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	virtual ~LRTTalon();
	virtual void SetDutyCycle(float speed);
	void Set(float speed);
	virtual float Get();
	virtual void Disable();
	virtual void PIDWrite(float output);
	
	virtual void ConfigNeutralMode(LRTSpeedController::NeutralMode mode);
	/*
	 * Writes the values to the Talon.
	 */
	void Update();
	
	virtual const char* GetName();
	
	static vector<LRTTalon*> talon_vector;
private:
	const char* m_name;
	float m_pwm;
	
	DigitalOutput* m_brake_jumper;
	LRTSpeedController::NeutralMode m_neutral;
};

#endif /* LRT_TALON_H_ */
