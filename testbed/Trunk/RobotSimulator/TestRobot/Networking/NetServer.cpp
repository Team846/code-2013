#include "NetServer.h"

#include "NetConnection.h"

NetServer::NetServer(int port)
	: NetPeer("", port, SERVER)
{
	
}

void NetServer::SendToAll(NetBuffer* buff, NetChannel::Enum method, int channel)
{
	vector<NetConnection*> netConnections = Connections();
	
	printf("connection count: %d", netConnections.size());
	for(vector<NetConnection*>::iterator it = netConnections.begin(); it != netConnections.end(); it++)
	{
		Send(buff, *it, method, channel);
	}

	printf("done sending\n");
}

vector<NetConnection*> NetServer::Connections()
{
	InternalPlatformConnectionListSynchronizationEnter();
	vector<NetConnection*> cloned = m_netConnections;
	InternalPlatformConnectionListSynchronizationLeave();
	
	return cloned;
}
