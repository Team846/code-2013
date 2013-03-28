#ifndef NET_CLIENT_H_
#define NET_CLIENT_H_

#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "NetPeer.h"

using namespace Network;

namespace Network
{
	class NetClient : public NetPeer
	{
	public:
		NetClient();
		
		void Connect(sockaddr_in ep);
	private:
		NetConnection* _server;
	};
};

#endif
