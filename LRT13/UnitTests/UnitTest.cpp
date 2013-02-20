#include "UnitTest.h"

UnitTest::UnitTest()
{ }

UnitTest::~UnitTest()
{ }

void UnitTest::StartCompetition()
{
	GetWatchdog().SetEnabled(false);
	
	Run();
}
