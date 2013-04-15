#ifndef NET_CLIENT_H_
#define NET_CLIENT_H_
//sdfjlk
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
		void CheckMessages();
		
		NetConnection* _server;
		
		double _connectionRequestTime;
		bool _connectionRequested;
	};
};

#endif
