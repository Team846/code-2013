#ifndef DRIVE_ENCODERS_UNIT_TEST_H_
#define DRIVE_ENCODERS_UNIT_TEST_H_

#include "../UnitTests/UnitTest.h"

#include "../ComponentData/DrivetrainData.h"
#include "../Sensors/DriveEncoders.h"

class DriveEncodersUnitTest : public UnitTest
{
public:
	DriveEncodersUnitTest();
	~DriveEncodersUnitTest();
	
	void Run();
	
private:
	DriveEncoders* m_driveEncoders;
};

#endif
