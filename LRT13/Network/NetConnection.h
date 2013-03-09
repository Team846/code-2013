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

#include "LibraryMessageType.h"

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

	/*!
	 * @brief Describes a message header used by the internal library.
	 */
	enum InternalMessageType
	{
		LIBRARY_DATA = 0x00,
		USER_DATA = 0x01,
	};
	
	struct MessageAwaitingACK
	{
		NetBuffer* buff;
		double sentTime;
		
		bool acknowledged;
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
		static const double kResendPacketTime; // TO-DO: make me configurable
		
#ifdef __VXWORKS__
		static INT32 InternalPlatformUpdateTaskWrapper(UINT32 instance);
		static INT32 InternalPlatformMessageVerificationTaskWrapper(UINT32 instance);
#endif
		
		void InternalPlatformQueueSynchronizationCreate();
		void InternalPlatformQueueSynchronizationEnter();
		void Update();
		void CheckMessages();
		void InternalPlatformQueueSynchronizationLeave();
		
		// reliable
		void InternalPlatformReliableUnorderedQueueSynchronizationCreate();
		void InternalPlatformReliableUnorderedQueueSynchronizationEnter();
		void InternalPlatformReliableUnorderedQueueSynchronizationLeave();
		
		// reliable sequenced
		void InternalPlatformReliableSequencedQueueSynchronizationCreate();
		void InternalPlatformReliableSequencedQueueSynchronizationEnter();
		void InternalPlatformReliableSequencedQueueSynchronizationLeave();
				
		// reliable in order
		void InternalPlatformReliableInOrderQueueSynchronizationCreate();
		void InternalPlatformReliableInOrderQueueSynchronizationEnter();
		void InternalPlatformReliableInOrderQueueSynchronizationLeave();
		
		void InternalPlatformCreateUpdateTasks();
		void InternalPlatformRunUpdateTasks();
		void InternalPlatformDestroyUpdateTasks();
		
		char* m_ip;
		int m_port;
		
#ifdef __VXWORKS__
		SEM_ID m_msgQueueMutex;
		
		SEM_ID m_reliableUnorderedQueueMutex, m_reliableSequencedQueueMutex, m_reliableInOrderQueueMutex;
		
		Task* m_internalUpdateTask;
		Task* m_internalMessageVerificationTask;
#endif
		queue<NetBuffer*> m_receivedMessages;
		
		NetConnectionType m_connType;
		
		bool m_isRunning;
		
		int m_socket;
		struct sockaddr_in m_remote_spec;
		
		MessageAwaitingACK* m_reliableUnordered[16]; // NetBuffer array - one for each subchannel
		MessageAwaitingACK* m_reliableSequenced[16]; // NetBuffer array - one for each subchannel
		MessageAwaitingACK* m_reliableOrdered[16]; // NetBuffer array - one for each subchannel
		
		int* m_lastUnreliableSequenced; // int array of length 16 ints - one for each channel 
		int* m_lastReliableSequenced; // int array of length 16 ints - one for each channel
		
		int m_currentReliableUnorderedCounter;
		int m_currentReliableSequencedCounter;
		int m_currentReliableOrderedCounter;
		int m_currentUnreliableSequencedCounter;
	};
};

#endif
