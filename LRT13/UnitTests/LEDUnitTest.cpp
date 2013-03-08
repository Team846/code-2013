#include "LEDUnitTest.h"

LEDUnitTest::LEDUnitTest() :
 UnitTest(), m_led(new DigitalOutput(1))
{
	
}

LEDUnitTest::~LEDUnitTest()
{
	delete m_led;
	m_led = NULL;
}

void LEDUnitTest::Run()
{
	double currentTime = GetTime();
	double newTime = GetTime();
	while(true)
	{
//		m_led->Set(1);
//		taskDelay(sysClkRateGet() / 100);
//		m_led->Set(0);
//		taskDelay(sysClkRateGet() / 100);
		printf("While loop rate: %.15lf\n", newTime - currentTime);
		currentTime = GetTime();
		newTime = GetTime();
//		taskDelay(sysClkRateGet());
	}
}
