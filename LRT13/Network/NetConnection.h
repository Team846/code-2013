#ifndef NET_CONNECTION_H_
#define NET_CONNECTION_H_

#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

//#include <netinet/in.h> // <-- DO NOT USE
//#include <arpa/inet.h> // <-- BLACK MAGIC

#include <iostream>

#include <WPILib.h>

//#include "NetBuffer.h"
#include "NetConnectionType.h"

#define SEND_FAILED_BUFFER_ALREADY_SENT -1000000000
#define SEND_FAILED_BUFFER_INVALID -1000000001
#define SEND_FAILED_UNKNOWN_ERROR -10000000002

using namespace std;
using namespace Network;

namespace Network
{
	class NetBuffer;

	/*!
	 * @brief Represents a network connection.
	 * @author Tony Peng
	 */
	class NetConnection
	{
	public:
		/*!
		 * @brief Sets up a network connection.
		 */
		NetConnection(char * ip, int port, NetConnectionType connType);
		/*!
		 * @brief Releases the resources used by the NetConnection.
		 */
		~NetConnection();
		
		/*!
		 * @brief Opens the network connection.
		 */
		int Open(int options=0, ...);
		/*!
		 * @brief Closes and disconnects the network connection.
		 */
		int Close();
		
		int Send(NetBuffer buff);
		
	private:
		char* m_ip;
		int m_port;
		
		NetConnectionType m_connType;
		
		int m_socket;
		struct sockaddr_in m_remote_spec;
	};
};

#endif
