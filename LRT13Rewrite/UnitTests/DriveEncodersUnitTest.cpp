#include "DriveEncodersUnitTest.h"

DriveEncodersUnitTest::DriveEncodersUnitTest()
	: UnitTest(),
	  m_driveEncoders(DriveEncoders::GetInstance())
{
	
}

DriveEncodersUnitTest::~DriveEncodersUnitTest()
{
	DriveEncoders::Finalize();
}

void DriveEncodersUnitTest::Run()
{
	while(true)
	{
		printf("Normalized forward: %lf \n", m_driveEncoders->getNormalizedForwardSpeed());
		printf("Normalized left speed: %lf \n", m_driveEncoders->getNormalizedSpeed(data::drivetrain::LEFT));
		printf("Normalized right speed: %lf \n", m_driveEncoders->getNormalizedSpeed(data::drivetrain::RIGHT));
		printf("Normalized turn speed: %lf", m_driveEncoders->getNormalizedTurningSpeed());
		
		Wait(5.0);
	}
}
