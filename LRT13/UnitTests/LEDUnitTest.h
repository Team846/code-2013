#ifndef LED_TEST_H_
#define LED_TEST_H_

#include <vxWorks.h>
#include <sysLib.h>

#include "../UnitTests/UnitTest.h"
#include "DigitalOutput.h"
#include <timer.h>

class LEDUnitTest : public UnitTest
{
public:
	LEDUnitTest();
	~LEDUnitTest();
	
	void Run();
	
private:
	DigitalOutput* m_led;
};

#endif
