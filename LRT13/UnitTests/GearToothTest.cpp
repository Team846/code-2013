#include "GearToothTest.h"

GearToothTest::GearToothTest() :
	UnitTest(),
	gearTooth(3)
{
	
}

void GearToothTest::Run()
{
	printf("Gear Tooth Sensor: %d\n", gearTooth.Get());
}
