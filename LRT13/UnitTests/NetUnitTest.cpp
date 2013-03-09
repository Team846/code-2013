#include "NetUnitTest.h"

NetUnitTest::NetUnitTest()
	: UnitTest()
{
	m_conn = new NetConnection("10.8.46.x", 2568, CLIENT);
}

void NetUnitTest::Run()
{
	m_conn->Open(0);
	
	int counter = 0;
	
	while(true)
	{
		NetBuffer b;
		
		stringstream s;
		
		s << "HI" << counter++;
		
		b.Write(s.str());
		
		m_conn->Send(b, NetChannel::NET_UNRELIABLE_SEQUENCED, 0);
		
		taskDelay(sysClkRateGet() / 4);
	}
}
