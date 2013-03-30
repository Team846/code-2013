#include "NetConnection.h"

using namespace Network;

sockaddr_in* NetConnection::RemoteEndpoint()
{
	return &m_remoteEndpoint;
}

NetConnection::NetConnection(sockaddr_in iep, NetPeer* peer)
{
	m_remoteEndpoint = iep;
	
	m_netPeer = peer;
}
		
void NetConnection::Send(NetBuffer* buff, NetChannel::Enum method, int channel)
{
	m_netPeer->Send(buff, this, method, channel);
}
