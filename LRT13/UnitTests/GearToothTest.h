#ifndef GEAR_TOOTH_TEST_H_
#define GEAR_TOOTH_TEST_H_

#include "../Utils/UnitTest.h"
#include "GearTooth.h"

class GearToothTest : public UnitTest
{
public:
	GearToothTest();
	void Run();
private:
	GearTooth gearTooth;
};

#endif
