#ifndef NET_CONNECTION_H_
#define NET_CONNECTION_H_

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
		
		sockaddr m_remoteEndpoint;
		NetPeer* m_netPeer;
		
	public:
		sockaddr* RemoteEndpoint();
		
		NetConnection(sockaddr iep, NetPeer* peer);
		
		void Send(NetBuffer buff, NetChannel::Enum method, int channel);
	};
};

#endif