#ifndef LRT_TALON_H_
#define LRT_TALON_H_

#include "Talon.h"
#include "DigitalOutput.h"
#include <vector>

using namespace std;

class LRTTalon : public Talon
{
public:
	typedef enum {kNeutralMode_Coast = 0, kNeutralMode_Brake = 1} NeutralMode;
	
	explicit LRTTalon(UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	LRTTalon(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	virtual ~LRTTalon();
	virtual void Set(float value);
	virtual float Get();
	virtual void Disable();
	virtual void PIDWrite(float output);
	
	void SetNeutralMode(NeutralMode mode);
	/*
	 * Writes the values to the Talon.
	 */
	void Update();
	
	const char* GetName();
	
	static vector<LRTTalon*> talon_vector;
private:
	const char* m_name;
	float m_pwm;
	
	DigitalOutput* m_brake_jumper;
	NeutralMode m_neutral;
};

#endif /* LRT_TALON_H_ */
