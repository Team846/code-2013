#ifndef PROXIMITY_TEST_H_
#define PROXIMITY_TEST_H_

#include "../UnitTests/UnitTest.h"
#include "DigitalInput.h"
#include "../Config/RobotConfig.h"

class ProximityTest : public UnitTest
{
public:
	ProximityTest();
	void Run();
private:
	DigitalInput* proximity;
};

#endif
