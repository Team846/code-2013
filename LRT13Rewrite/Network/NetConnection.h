#ifndef NET_CONNECTION_H_
#define NET_CONNECTION_H_
//sdfjl
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "NetPeer.h"

namespace Network
{
	class NetConnection
	{
	private:
		int m_socket;
		
		sockaddr_in m_remoteEndpoint;
		NetPeer* m_netPeer;
		
	public:
		sockaddr_in* RemoteEndpoint();
		
		NetConnection(sockaddr_in iep, NetPeer* peer);
		
		void Send(NetBuffer* buff, NetChannel::Enum method, int channel);
	};
};

#endif
