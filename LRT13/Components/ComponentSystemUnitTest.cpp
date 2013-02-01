#include "ComponentSystemUnitTest.h"

ComponentSystemUnitTest::ComponentSystemUnitTest()
	: Component("ComponentSystemUnitTest", DriverStationConfig::DigitalIO::COMP_UNIT_TEST, true)
{
	AsyncPrinter::Println("ComponentSystemUnitTest::ctor()");
}

ComponentSystemUnitTest::~ComponentSystemUnitTest()
{
	AsyncPrinter::Println("ComponentSystemUnitTest::dtor()");
	AsyncPrinter::Println("ComponentSystemUnitTest::dtor(): In a real component, resources would be freed...");
}

void ComponentSystemUnitTest::onEnable()
{
	AsyncPrinter::Println("ComponentSystemUnitTest::onEnable()");
}

void ComponentSystemUnitTest::onDisable()
{
	AsyncPrinter::Println("ComponentSystemUnitTest::onDisable()");
}

void ComponentSystemUnitTest::enabledPeriodic()
{
	static int e = 0;
	
	if(++e % RobotConfig::LOOP_RATE == 0)
	{
		AsyncPrinter::Println("ComponentSystemUnitTest::whenEnabled(): Tick %d", e);
	}
}

void ComponentSystemUnitTest::disabledPeriodic()
{
	static int e = 0;
	
	if(++e % RobotConfig::LOOP_RATE == 0)
	{
		AsyncPrinter::Println("ComponentSystemUnitTest::whenDisabled(): Tick %d", e);
	}
}
