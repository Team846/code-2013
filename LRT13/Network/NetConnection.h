#ifndef NET_CONNECTION_H_
#define NET_CONNECTION_H_

#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

//#include <netinet/in.h> // <-- DO NOT USE
//#include <arpa/inet.h> // <-- BLACK MAGIC

#include <iostream>
#include <sstream>
#include <queue>

#include <WPILib.h>

#ifdef __VXWORKS__
#include <Task.h>
#endif

//#include "NetBuffer.h"
#include "NetConnectionType.h"
#include "NetChannel.h"

#define SEND_FAILED_BUFFER_ALREADY_SENT -1000000000
#define SEND_FAILED_BUFFER_INVALID -1000000001
#define SEND_FAILED_UNKNOWN_ERROR -10000000002

#define MAX_RECEIVE_BUFFER_SIZE 1024
#define MAX_MESSAGE_TRACK 256 // 256 gives us ample time to wait for an ACK. ~ 5 messages per frame * 50 frames per second gives us 6 extra packets.

using namespace std;
using namespace Network;

namespace Network
{
	class NetBuffer;

	enum LibraryMessageType
	{
		LIBRARY_DATA = 0x00,
		USER_DATA = 0x01,
	};
	
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
		
		/*!
		 * @brief Sends the contents of the NetBuffer over the network connection.
		 */
		int Send(NetBuffer buff, NetChannel::Enum method, int channel);
		
		/*!
		 * @brief Returns the next message in the message queue.
		 */
		NetBuffer* GetMessage();
	protected:
		INT32 Tick();
	private:
#ifdef __VXWORKS__
		static INT32 InternalPlatformUpdateTaskWrapper(UINT32 instance);
#endif
		
		void InternalPlatformQueueSynchronizationCreate();
		void InternalPlatformQueueSynchronizationEnter();
		void Update();
		void InternalPlatformQueueSynchronizationLeave();
		
		void InternalPlatformCreateUpdateTask();
		void InternalPlatformRunUpdateTask();
		void InternalPlatformDestroyUpdateTask();
		
		char* m_ip;
		int m_port;
		
#ifdef __VXWORKS__
		SEM_ID m_msgQueueMutex;
		
		Task* m_internalUpdateTask;
#endif
		queue<NetBuffer*> m_receivedMessages;
		
		NetConnectionType m_connType;
		
		bool m_isRunning;
		
		int m_socket;
		struct sockaddr_in m_remote_spec;
		
		NetBuffer** m_reliableUnordered;
		NetBuffer** m_reliableSequenced;
		NetBuffer** m_reliableOrdered;
		
		int* m_lastUnreliableSequenced;
		int* m_lastReliableSequenced;
		
		int m_currentReliableUnorderedCounter;
		int m_currentReliableSequencedCounter;
		int m_curerntReliableOrderedCounter;
	};
};

#endif
