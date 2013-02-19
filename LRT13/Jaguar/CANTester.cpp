#include "CANTester.h"
#include "../Config/RobotConfig.h"
#include "CANJaguar.h"
#include "Utility.h"
#include "Timer.h"


CANTester::CANTester() :
m_twocanMosfet(RobotConfig::Digital::TWOCAN_MOSFET)
{
	double voltage;
	{
		CANJaguar jaggie(2); //One of the drive jaguars
		wasteTimeUntil(m_startUpTime);
		voltage = jaggie.GetBusVoltage();
	}
	
	while (voltage < 5)
	{
		m_twocanMosfet.Set(0);
		Wait(1.0);
		m_twocanMosfet.Set(1);
		wasteTimeUntil(GetFPGATime() + m_startUpTime);
		CANJaguar jaggie(2); //One of the drive jaguars
		voltage = jaggie.GetBusVoltage();
	} 
};

void CANTester::wasteTimeUntil( unsigned int FPGATargetTime)
{
	int useless = 0;
	while (GetFPGATime() < FPGATargetTime)
	{
		useless++;
	}
};
