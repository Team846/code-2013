#ifndef NET_PEER_H_
#define NET_PEER_H_
//sdfjkl
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <algorithm>

//#include <netinet/in.h> // <-- DO NOT USE
//#include <arpa/inet.h> // <-- BLACK MAGIC

#include <iostream>
#include <sstream>
#include <queue>
#include <map>
#include <stdarg.h>

#include "Includes.h"

#ifdef __VXWORKS__
#include <Task.h>
#else #ifdef USE_BOOST
#include <boost/thread.hpp>
#include <boost/signals2/mutex.hpp>
#endif

#include "NetBuffer.h"
#include "NetConnectionType.h"
#include "NetChannel.h"

#include "LibraryMessageType.h"

// sleep for X milliseconds
#ifdef __VXWORKS__
#define NET_SLEEP(x) Wait (x / 1000.0)
#else
#define NET_SLEEP(x) usleep(x * 1000.0);
#endif

#define SEND_FAILED_BUFFER_ALREADY_SENT -1000000000
#define SEND_FAILED_BUFFER_INVALID -1000000001
#define SEND_FAILED_UNKNOWN_ERROR -10000000002

#define MAX_RECEIVE_BUFFER_SIZE 1024
#define MAX_MESSAGE_TRACK 256 // 256 gives us ample time to wait for an ACK. ~ 5 messages per frame * 50 frames per second gives us 6 extra packets.

using namespace std;
using namespace Network;

namespace Network
{
	class NetConnection;

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
		bool initialized;
		
		NetBuffer* buff;
		double sentTime;
		
		bool acknowledged;
		
		NetConnection* recipient;
	};
	
	/*!
	 * @brief Represents a network connection.
	 * @author Tony Peng
	 */
	class NetPeer
	{
	public:
		/*!
		 * @brief Sets up a network connection.
		 */
		NetPeer(char * ip, int port, NetConnectionType connType);
		/*!
		 * @brief Releases the resources used by the NetConnection.
		 */
		~NetPeer();
		
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
		int Send(NetBuffer* buff, NetConnection* to, NetChannel::Enum method, int channel, int id=-1);
		
		/*!
		 * @brief Returns the next message in the message queue.
		 */
		NetBuffer* ReadMessage();
	protected:
		void SendRaw(NetBuffer* nb, NetConnection* nc);
		virtual void CheckMessages();
		
		bool _connected;
		
		vector<NetConnection*> m_netConnections;
		void InternalPlatformConnectionListSynchronizationEnter();
		void InternalPlatformConnectionListSynchronizationLeave();
	private:
		static const double kResendPacketTime; // TO-DO: make me configurable
		
#ifdef __VXWORKS__
		static INT32 InternalPlatformUpdateTaskWrapper(UINT32 instance);
		static INT32 InternalPlatformMessageVerificationTaskWrapper(UINT32 instance);

#else #ifdef USE_BOOST
		static INT32 InternalPlatformUpdateTaskWrapper(NetPeer* instance);
		static INT32 InternalPlatformMessageVerificationTaskWrapper(NetPeer* instance);		
#endif
		
		void InternalPlatformQueueSynchronizationCreate();
		void InternalPlatformQueueSynchronizationEnter();
		void Update();
		void InternalPlatformQueueSynchronizationLeave();
	
		void InternalPlatformConnectionListSynchronizationCreate();
		
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
		
		SEM_ID m_connectionListMutex;
		
		SEM_ID m_reliableUnorderedQueueMutex, m_reliableSequencedQueueMutex, m_reliableInOrderQueueMutex;
		
		Task* m_internalUpdateTask;
		Task* m_internalMessageVerificationTask;
		
#else #ifdef USE_BOOST
		boost::signals2::mutex* m_msgQueueMutex;
		boost::signals2::mutex* m_connectionListMutex;
		boost::signals2::mutex* m_reliableUnorderedQueueMutex, *m_reliableSequencedQueueMutex, *m_reliableInOrderQueueMutex;

		boost::thread* m_internalUpdateTask;
		boost::thread* m_internalMessageVerificationTask;
#endif
		
		queue<NetBuffer*> m_receivedMessages;
		
		NetConnectionType m_connType;
		
		bool m_isRunning;
		
		int m_socket;
		sockaddr_in m_socketEndpoint;

		map<int, MessageAwaitingACK> m_reliableUnordered[16];
		map<int, MessageAwaitingACK> m_reliableSequenced[16];
		map<int, MessageAwaitingACK> m_reliableOrdered[16];
		
		int* m_lastUnreliableSequenced; // int array of length 16 ints - one for each channel 
		int* m_lastReliableSequenced; // int array of length 16 ints - one for each channel
		
		int m_currentReliableUnorderedCounter;
		int m_currentReliableSequencedCounter;
		int m_currentReliableOrderedCounter;
		int m_currentUnreliableSequencedCounter;
	};
};

#endif
