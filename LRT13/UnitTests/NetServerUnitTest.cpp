#include "NetServerUnitTest.h"

NetServerUnitTest::NetServerUnitTest()
	: UnitTest()
{
	printf("Creating NetConnection\n");
	m_conn = new NetConnection(INADDR_ANY, 2568, SERVER);
}

void NetServerUnitTest::Run()
{
	printf("Run!\n");
	
	m_conn->Open(0);
	
	int counter = 0;
	
	printf("Before while loop\n");
	
	while(true)
	{
		NetBuffer b;
		
		stringstream s;

		s << "HI" << counter++;

		b.Write(s.str());
		
		m_conn->Send(b, NetChannel::NET_UNRELIABLE_SEQUENCED, 0);
		
		printf("Sending packet...%d\n", counter);
		
		taskDelay(sysClkRateGet());
	}
}
