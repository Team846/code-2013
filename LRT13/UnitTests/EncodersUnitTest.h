#ifndef ENCODER_UNIT_TEST_H_
#define ENCODER_UNIT_TEST_H_

#include "../UnitTests/UnitTest.h"
#include "../Config/RobotConfig.h"
#include "../Config/ConfigManager.h"

class EncodersUnitTest : public UnitTest
{
public:
	EncodersUnitTest();
	~EncodersUnitTest();
	
	void Run();
	
private:
	Encoder m_leftEncoder;
	Encoder m_rightEncoder;
};

#endif
