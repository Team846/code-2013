#include "ProximityTest.h"
#include <vxWorks.h>
#include <sysLib.h>

ProximityTest::ProximityTest() :
	UnitTest()
{
	proximity = new DigitalInput((UINT32)RobotConfig::Digital::PROXIMITY_SHOOTER);
}

void ProximityTest::Run()
{
	while(true)
	{
		printf("Proximity Sensor: %d\n", proximity->Get());
		taskDelay(sysClkRateGet() / 4);
	}
}
