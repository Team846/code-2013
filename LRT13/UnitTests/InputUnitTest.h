#ifndef INPUT_UNIT_TEST_H_
#define INPUT_UNIT_TEST_H_

#include <vxWorks.h>
#include <sysLib.h>

#include "UnitTest.h"

#include "../Config/DriverStationConfig.h"
#include "../Operations/DebouncedJoystick.h"
#include "../Pneumatics/Pneumatics.h"

class InputUnitTest : public UnitTest
{
public:
	InputUnitTest();
	~InputUnitTest();
	
	void Run();
	
private:
	DebouncedJoystick* m_stick;
	DebouncedJoystick* m_wheel;
};

#endif
