#include "GearToothTest.h"
#include <vxWorks.h>
#include <sysLib.h>

GearToothTest::GearToothTest() :
	UnitTest()
{
	gearTooth = new Counter((UINT32)RobotConfig::Digital::WINCH_GEAR_TOOTH);
}

void GearToothTest::Run()
{
	while(true)
	{
		printf("Gear Tooth Sensor period: %lf\n", gearTooth->GetPeriod());
		printf("Gear Tooth Sensor: %d\n", gearTooth->Get());
		taskDelay(sysClkRateGet() / 4);
	}
}
