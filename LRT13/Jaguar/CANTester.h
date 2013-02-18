#ifndef CANTester_H_
#define CANTester_H_
#include "DigitalOutput.h"

class CANTester
{
public:
	CANTester();
	
private:
	DigitalOutput m_twocanMosfet;
	int m_startUpTime;
	void wasteTimeUntil(unsigned int FPGATargetTime);
};

#endif /* CANTester_H_ */
