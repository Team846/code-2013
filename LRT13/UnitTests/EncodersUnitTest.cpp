#include "EncodersUnitTest.h"
#include <vxWorks.h>
#include <sysLib.h>

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
	double maxLeft = 0;
	double maxRight = 0;
	
	while(true)
	{
		INT32 leftEncoderValue = m_leftEncoder.Get();
		INT32 rightEncoderValue = m_rightEncoder.Get();
		
		double leftEncoderRate = m_leftEncoder.GetRate();
		double rightEncoderRate = m_rightEncoder.GetRate();
		
		maxLeft = max(leftEncoderRate, maxLeft);
		maxRight = max(rightEncoderRate, maxRight);
			
		printf("Left Encoder Value: %d \n", m_leftEncoder.Get());
		printf("Right Encoder Value: %d \n", m_rightEncoder.Get());
		
		printf("Left Encoder Rate: %lf \n", m_leftEncoder.GetRate());
		printf("Right Encoder Rate: %lf \n", m_rightEncoder.GetRate());
		
		printf("Max left rate: %lf \n", maxLeft);
		printf("Max right rate: %lf \n", maxRight);
		
		taskDelay(sysClkRateGet() / 4);
	}
}
