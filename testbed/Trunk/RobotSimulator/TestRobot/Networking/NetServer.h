#ifndef NET_SERVER_H_
#define NET_SERVER_H_

#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <semLib.h>

#include "NetPeer.h"
#include "NetChannel.h"

using namespace Network;

namespace Network
{
	class NetServer : public NetPeer
	{
	public:
		NetServer(int port);
		
		void SendToAll(NetBuffer* buff, NetChannel::Enum method, int channel);
		
		vector<NetConnection*> Connections(); 
	};
};

#endif
