#include "AutoAim.h"

AutoAim::AutoAim()
{
	m_conn = new NetPeer(INADDR_ANY, 8000, Network::SERVER);
}

AutoAim::~AutoAim()
{
	DELETE(m_conn);
}

void AutoAim::Update()
{
	NetBuffer* msg;
	
	while((msg = m_conn->ReadMessage()) != NULL)
	{
		// process packets
		
		DELETE(msg);
	}
}
