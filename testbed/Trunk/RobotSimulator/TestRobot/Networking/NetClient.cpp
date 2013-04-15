#include "NetClient.h"
#include "NetConnection.h"
#include "NetBuffer.h"
//sdf
NetClient::NetClient()
	: NetPeer("0.0.0.0", 0, CLIENT)
{
	
}

void NetClient::Connect(sockaddr_in ep)
{
	if(_connectionRequested)
		return;
	
#ifdef __VXWORKS__
	_connectionRequestTime = Timer::GetFPGATimestamp();
#else
	_connectionRequestTime = clock() / (double) CLOCKS_PER_SEC;
#endif
	_connectionRequested = true;
	
	_server = new NetConnection(ep, this);
	
	NetBuffer netBuffer;
	
	netBuffer.Write((char)LIBRARY_DATA);
	netBuffer.Write((char)LibraryMessageType::CONNECTION_REQUEST);
	
	SendRaw(&netBuffer, _server);
}
