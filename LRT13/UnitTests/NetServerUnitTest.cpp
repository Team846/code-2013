#include "NetServerUnitTest.h"

NetServerUnitTest::NetServerUnitTest()
	: UnitTest()
{
	m_conn = new NetConnection("10.8.46.x", 2568, SERVER);
}

void NetServerUnitTest::Run()
{
	m_conn->Open(0);
	
	int counter = 0;
	
	while(true)
	{
		NetBuffer b;
		
		stringstream s;
		
		s << "HI" << counter++;
		
		b.Write(s.str());
		
		taskDelay(sysClkRateGet() / 4);
	}
}
