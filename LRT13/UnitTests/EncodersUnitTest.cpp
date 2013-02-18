#include "EncodersUnitTest.h"

EncodersUnitTest::EncodersUnitTest()
	: UnitTest(),
	  m_leftEncoder(RobotConfig::Digital::LEFT_DRIVE_ENCODER_A, RobotConfig::Digital::LEFT_DRIVE_ENCODER_B, false, CounterBase::k4X),
	  m_rightEncoder(RobotConfig::Digital::RIGHT_DRIVE_ENCODER_A, RobotConfig::Digital::RIGHT_DRIVE_ENCODER_B, false, CounterBase::k4X)
{
	m_leftEncoder.Start();
	m_leftEncoder.SetMinRate(10.0);
	
	m_rightEncoder.Start();
	m_rightEncoder.SetMinRate(10.0);
}

EncodersUnitTest::~EncodersUnitTest()
{
	
}

void EncodersUnitTest::Run()
{
	printf("Left Encoder Value: %d \n", m_leftEncoder.Get());
	printf("Right Encoder Value: %d \n", m_rightEncoder.Get());
	
	printf("Left Encoder Rate: %lf \n", m_leftEncoder.GetRate());
	printf("Right Encoder Rate: %lf \n", m_rightEncoder.GetRate());
}
