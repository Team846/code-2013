#include "DiscLocator.h"

DiscLocator::DiscLocator()
{
	m_conn = new NetPeer(INADDR_ANY, 8000, Network::SERVER);
}

DiscLocator::~DiscLocator()
{
	DELETE(m_conn);
}

void DiscLocator::Update()
{
	NetBuffer* msg;
	
	while((msg = m_conn->ReadMessage()) != NULL)
	{
		// process packets
		
		DELETE(msg);
	}
}
