#include "NetServerUnitTest.h"

NetServerUnitTest::NetServerUnitTest()
	: UnitTest()
{
	printf("Creating NetConnection\n");
	m_conn = new NetServer(846);
}

void NetServerUnitTest::Run()
{
	printf("Run!\n");
	
	m_conn->Open();
	
	int counter = 0;
	float totalTime = 0;
	
	printf("Before while loop\n");
	
	while(true)
	{
		NetBuffer b;

		float value = (float)fabs(sin((float)totalTime / 1000.0f));
		
		std::stringstream ss;
		
		ss << counter;
		
		b.Write((char)MessageType::ROBOT_TELEMETRY);
		b.Write(ss.str());
		
		m_conn->SendToAll(&b, NetChannel::NET_UNRELIABLE_SEQUENCED, 1);
		//m_conn->Send(b, NetChannel::NET_UNRELIABLE_SEQUENCED, 0);
		
		//printf("Sending packet...%d\n", counter);
		
		if(counter++ % 10)
			printf("Tick: %d", counter - 1);
		
		taskDelay(sysClkRateGet() / 10);
		totalTime += 10;
	}
}
