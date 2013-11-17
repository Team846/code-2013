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
	
	_connectionRequestTime = TIME_IN_SECONDS();
	_connectionRequested = true;
	
	_server = new NetConnection(ep, this);
	
	NetBuffer netBuffer;
	
	netBuffer.Write((UINT8)LIBRARY_DATA);
	netBuffer.Write((UINT8)LibraryMessageType::CONNECTION_REQUEST);
	
	SendRaw(&netBuffer, _server);
}

void NetClient::CheckMessages()
{
	NetPeer::CheckMessages();
	
	if(_connectionRequested && !_connected)
	{
		double timeNow = TIME_IN_SECONDS();
		
		if((timeNow - _connectionRequestTime) > 1.00)
		{
			NetBuffer netBuffer;
				
			netBuffer.Write((UINT8)LIBRARY_DATA);
			netBuffer.Write((UINT8)LibraryMessageType::CONNECTION_REQUEST);
			
			SendRaw(&netBuffer, _server);
			
			_connectionRequestTime = timeNow;
		}
	}
}
