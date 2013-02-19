#ifndef GEAR_TOOTH_TEST_H_
#define GEAR_TOOTH_TEST_H_

#include "../UnitTests/UnitTest.h"
#include "Counter.h"
#include "../Config/RobotConfig.h"

class GearToothTest : public UnitTest
{
public:
	GearToothTest();
	void Run();
private:
	Counter* gearTooth;
};

#endif
