#include "NetBuffer.h"
#include "NetConnection.h"

const double NetConnection::kResendPacketTime = 1.0; // resend if 100 ms passes with no ACK; max of 500ms roundtrip time

NetConnection::NetConnection(char * ip, int port, NetConnectionType connType)
{
	this->m_ip = ip;
	this->m_port = port;
	this->m_connType = connType;
	
	for(int i = 0; i < 16; i++)
	{
		m_reliableOrdered[i] = new MessageAwaitingACK[MAX_MESSAGE_TRACK];
		m_reliableSequenced[i] = new MessageAwaitingACK[MAX_MESSAGE_TRACK];
		m_reliableOrdered[i] = new MessageAwaitingACK[MAX_MESSAGE_TRACK];
	}
	
	this->m_lastUnreliableSequenced = new int[MAX_MESSAGE_TRACK];
	this->m_lastReliableSequenced 	= new int[MAX_MESSAGE_TRACK];
	
	this->m_currentReliableUnorderedCounter = 0;
	this->m_currentReliableSequencedCounter = 0;
	this->m_currentReliableOrderedCounter = 0;
	this->m_currentUnreliableSequencedCounter = 0;
	
	InternalPlatformQueueSynchronizationCreate();
	
	InternalPlatformReliableUnorderedQueueSynchronizationCreate();
	InternalPlatformReliableSequencedQueueSynchronizationCreate();
	InternalPlatformReliableInOrderQueueSynchronizationCreate();
}

NetConnection::~NetConnection()
{
	Close();
}

#ifdef __VXWORKS__
INT32 NetConnection:: InternalPlatformUpdateTaskWrapper(UINT32 instance)
{
	// vxworks specific code
	
	NetConnection* conn = (NetConnection*)instance;
	
	while(conn->m_isRunning)
	{
		conn->Update();
	}
	
	return 0;
}

INT32 NetConnection::InternalPlatformMessageVerificationTaskWrapper(UINT32 instance)
{
	// vxworks specific code
	
	NetConnection* conn = (NetConnection*)instance;
	
	while(conn->m_isRunning)
	{
		conn->CheckMessages();
	}
	
	return 0;
}
#endif

void NetConnection::InternalPlatformQueueSynchronizationCreate()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_msgQueueMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
#endif
}

void NetConnection::InternalPlatformQueueSynchronizationEnter()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semTake(m_msgQueueMutex, WAIT_FOREVER);
#endif
}

void NetConnection::Update()
{
	int received;
	char rcv_buffer[MAX_RECEIVE_BUFFER_SIZE];
	int addr_size = sizeof(m_remote_spec);
	
	while((received = recvfrom(m_socket, rcv_buffer, MAX_RECEIVE_BUFFER_SIZE, 0, (struct sockaddr*)&m_remote_spec, &addr_size)) > 0)
	{
		NetBuffer* buff = new NetBuffer(rcv_buffer, received);
		
		InternalMessageType c = (InternalMessageType)buff->ReadChar();
		
		switch(c)
		{
		case LIBRARY_DATA:
			// what message?
			LibraryMessageType::Enum msgType = (LibraryMessageType::Enum)buff->ReadChar();
			
			switch(msgType)
			{
			case LibraryMessageType::MESSAGE_ACK:
				// first byte is the send type
				NetChannel::Enum chann = (NetChannel::Enum)buff->ReadChar();
				
				// second byte is the channel
				int channel = buff->ReadChar();
				
				// 2, 3, 4, 5 (4 bytes) form the packet id
				int id = buff->ReadInt32();
				
				switch(chann)
				{
				case NetChannel::NET_RELIABLE:
					m_reliableUnordered[channel][id].acknowledged = true;
					break;
				case NetChannel::NET_RELIABLE_IN_ORDER:
					m_reliableOrdered[channel][id].acknowledged = true;
					break;
				case NetChannel::NET_RELIABLE_SEQUENCED:
					m_reliableSequenced[channel][id].acknowledged = true;
					break;
				}
				
				printf("Received message ack on channel %d with send method %d.\n", channel, chann);
				break;
			}
			break;
		case USER_DATA:
		{
			// read in the library header data first
			
			// first byte is the send type
			NetChannel::Enum chann = (NetChannel::Enum)buff->ReadChar();
			// second byte is the channel
			int channel = buff->ReadChar();
			// 2, 3, 4, 5 (4 bytes) form the packet id
			int id = buff->ReadInt32();
			
			// we only need to ACK back reliable packets
			switch(chann)
			{
			case NetChannel::NET_RELIABLE_SEQUENCED:
			case NetChannel::NET_RELIABLE_IN_ORDER:
			case NetChannel::NET_RELIABLE:
				// reliable needs an ACK
				NetBuffer ack;
				
				ack.Write((char)LIBRARY_DATA);
				ack.Write((char)chann);
				ack.Write((char)channel);
				ack.Write(id);
				
				// TODO error handling in the future
				sendto(m_socket, ack.GetBuffer(), ack.GetBytePos(), 0, (struct sockaddr*) &m_remote_spec, sizeof(m_remote_spec));

				break;
			}
			
			bool receive = true;
			int lastPacket;
			
			// handle sequencing
			switch(chann)
			{
			case NetChannel::NET_UNRELIABLE_SEQUENCED:
				lastPacket = m_lastUnreliableSequenced[channel];
				
				if(id < lastPacket) // TODO: rollover will break this
					receive = false;
				else
					m_lastUnreliableSequenced[channel] = id;
				break;
			case NetChannel::NET_RELIABLE_SEQUENCED:
				lastPacket = m_lastReliableSequenced[channel];
								
				if(id < lastPacket) // TODO: rollover will break this
					receive = false;
				else
					m_lastReliableSequenced[channel] = id;
				break;
			case NetChannel::NET_RELIABLE_IN_ORDER:
				receive = false; // will do this later
				break;
			}
			
			if(receive)
			{
				// synchronize on the semaphore so that we make sure we're safely accessing the internal message queue
				InternalPlatformQueueSynchronizationEnter();
				m_receivedMessages.push(buff);
				InternalPlatformQueueSynchronizationLeave(); // release the lock on the queue
			}
			break;
		}
		default:
			// wtf?
			break;
		}
	}
}

void NetConnection::CheckMessages()
{
	for(int channel = 0; channel < 16; channel++)
	{
		for(int i = 0; i < MAX_MESSAGE_TRACK; i++)
		{
			// reliable
			InternalPlatformReliableUnorderedQueueSynchronizationEnter();
			MessageAwaitingACK maack = m_reliableUnordered[channel][i];
			
			double now = Timer::GetFPGATimestamp();
			
			if(!maack.acknowledged && now - maack.sentTime > kResendPacketTime)
			{
				// mark this one as received so we don't send duplicates
				m_reliableUnordered[channel][i].acknowledged = true;
				
				// resend
				Send(*maack.buff, NetChannel::NET_RELIABLE, channel);
			}
			
			InternalPlatformReliableUnorderedQueueSynchronizationLeave();
			
			// update timestamp
			now = Timer::GetFPGATimestamp();
			
			InternalPlatformReliableSequencedQueueSynchronizationEnter();
			
			// check reliable sequenced
			maack = m_reliableSequenced[channel][i];
			
			if(!maack.acknowledged && now - maack.sentTime > kResendPacketTime)
			{
				// mark this one as received so we don't send duplicates
				m_reliableSequenced[channel][i].acknowledged = true;
				
				// resend
				Send(*maack.buff, NetChannel::NET_RELIABLE_SEQUENCED, channel);
			}
			
			InternalPlatformReliableSequencedQueueSynchronizationLeave();
			
			// update timestamp
			now = Timer::GetFPGATimestamp();
			
			InternalPlatformReliableInOrderQueueSynchronizationEnter();
			
			// check reliable in order
			maack = m_reliableOrdered[channel][i];
						
			if(!maack.acknowledged && now - maack.sentTime > kResendPacketTime)
			{
				// mark this one as received so we don't send duplicates
				m_reliableOrdered[channel][i].acknowledged = true;
				
				// resend
				Send(*maack.buff, NetChannel::NET_RELIABLE_IN_ORDER, channel);
			}
			
			InternalPlatformReliableInOrderQueueSynchronizationLeave();
		}
	}
}

void NetConnection::InternalPlatformQueueSynchronizationLeave()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semGive(m_msgQueueMutex);
#endif
}

void NetConnection::InternalPlatformReliableUnorderedQueueSynchronizationCreate()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_reliableUnorderedQueueMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
#endif
}

void NetConnection::InternalPlatformReliableUnorderedQueueSynchronizationEnter()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semTake(m_reliableUnorderedQueueMutex, WAIT_FOREVER);
#endif
}

void NetConnection::InternalPlatformReliableUnorderedQueueSynchronizationLeave()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semGive(m_reliableUnorderedQueueMutex);
#endif
}

void NetConnection::InternalPlatformReliableSequencedQueueSynchronizationCreate()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_reliableSequencedQueueMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
#endif
}

void NetConnection::InternalPlatformReliableSequencedQueueSynchronizationEnter()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semTake(m_reliableSequencedQueueMutex, WAIT_FOREVER);
#endif
}

void NetConnection::InternalPlatformReliableSequencedQueueSynchronizationLeave()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semGive(m_reliableSequencedQueueMutex);
#endif
}

void NetConnection::InternalPlatformReliableInOrderQueueSynchronizationCreate()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_reliableInOrderQueueMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
#endif
}

void NetConnection::InternalPlatformReliableInOrderQueueSynchronizationEnter()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semTake(m_reliableInOrderQueueMutex, WAIT_FOREVER);
#endif
}

void NetConnection::InternalPlatformReliableInOrderQueueSynchronizationLeave()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semGive(m_reliableInOrderQueueMutex);
#endif
}

void NetConnection::InternalPlatformCreateUpdateTasks()
{
	static int counter = 0;
	
	stringstream s;
	
	s << "NetConnection" << counter;
	
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalUpdateTask = new Task(s.str().c_str(), (FUNCPTR)InternalPlatformUpdateTaskWrapper);
#endif
	
	s << "_MessageCheck";
	
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalMessageVerificationTask = new Task(s.str().c_str(), (FUNCPTR)InternalPlatformMessageVerificationTaskWrapper);
#endif
}

void NetConnection::InternalPlatformRunUpdateTasks()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalUpdateTask->Start((UINT32)this);
	m_internalMessageVerificationTask->Start((UINT32)this);
#endif
}

void NetConnection::InternalPlatformDestroyUpdateTasks()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalUpdateTask->Stop();
	m_internalMessageVerificationTask->Stop();
	
	DELETE(m_internalUpdateTask);
	DELETE(m_internalMessageVerificationTask);
#endif
}

int NetConnection::Open(int options, ...)
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	if(m_socket < 0)
	{
		Close();
		return m_socket;
	}
	
	va_list vl;
	
	va_start(vl, options);
	
	int i;
	int value;
	
	for(i = 0; i < options; i++)
	{
		value = va_arg(vl, int); // do not be alarmed if the IDE states that this is a syntax error.
		
		// before you freak out about the scope of the pointer, the pointer is deref'd once by the system and never called again.
		char enabled = 1;
		setsockopt(m_socket, SOL_SOCKET, value, &enabled, sizeof(enabled));
	}
	
	va_end(vl);
	
	switch(this->m_connType)
	{
		case Network::CLIENT:
			m_remote_spec.sin_addr.s_addr = inet_addr(this->m_ip);
			m_remote_spec.sin_family = AF_INET;
			m_remote_spec.sin_port = htons(this->m_port);
			break;
		case Network::SERVER:
			m_remote_spec.sin_addr.s_addr = htonl(atoi(this->m_ip));
			m_remote_spec.sin_family = AF_INET;
			m_remote_spec.sin_port = htons(this->m_port);
			
			int retcode = bind(m_socket, (struct sockaddr*) &m_remote_spec, sizeof(m_remote_spec));
			
			if(retcode < 0)
			{
				Close();
				return retcode;
			}
			break;
	}
	
	m_isRunning = true;
	
	InternalPlatformCreateUpdateTasks();
	InternalPlatformRunUpdateTasks();

	return 0;
}

int NetConnection::Close()
{
	m_isRunning = false;
	
#warning this function is potentially bad because we might be killing the thread while it's waiting for a message. -tp
	InternalPlatformDestroyUpdateTasks();
	
	return close(m_socket);
}

int NetConnection::Send(NetBuffer buff, NetChannel::Enum method, int channel)
{
	if(buff.m_sent)
	{
		return SEND_FAILED_BUFFER_ALREADY_SENT;
	}

	if(buff.m_internalBuffer == NULL)
	{
		return SEND_FAILED_BUFFER_INVALID;
	}

	MessageAwaitingACK maack;
	
	maack.buff = &buff;
	maack.sentTime = Timer::GetFPGATimestamp();
	maack.acknowledged = false;
	
	NetBuffer* localBuff = new NetBuffer();
	localBuff->Write((char)USER_DATA);
	localBuff->Write((char)method);
	localBuff->Write((char)channel);
	
	switch(method)
	{
		// TODO: specific handlers for different methods
	case NetChannel::NET_RELIABLE:
		localBuff->Write(m_currentReliableUnorderedCounter);
		InternalPlatformReliableUnorderedQueueSynchronizationEnter();
		m_reliableUnordered[channel][m_currentReliableUnorderedCounter++] = maack;
		InternalPlatformReliableUnorderedQueueSynchronizationLeave();
		break;
	case NetChannel::NET_RELIABLE_IN_ORDER:
		localBuff->Write(m_currentReliableOrderedCounter);
		InternalPlatformReliableInOrderQueueSynchronizationEnter();
		m_reliableOrdered[channel][m_currentReliableOrderedCounter++] = maack;
		InternalPlatformReliableInOrderQueueSynchronizationLeave();
		break;
	case NetChannel::NET_RELIABLE_SEQUENCED:
		localBuff->Write(m_currentReliableSequencedCounter);
		InternalPlatformReliableSequencedQueueSynchronizationEnter();
		m_reliableSequenced[channel][m_currentReliableSequencedCounter++] = maack;
		InternalPlatformReliableSequencedQueueSynchronizationLeave();
		break;
	case NetChannel::NET_UNRELIABLE:
		localBuff->Write(0);
		 break;
	case NetChannel::NET_UNRELIABLE_SEQUENCED:
		localBuff->Write(m_currentUnreliableSequencedCounter++);
		break;
	default:

		break;
	}

	localBuff->Write(buff.m_internalBuffer);
	
	buff.m_sent = true;
	
	int iResult = sendto(m_socket, buff.m_internalBuffer, buff.GetBytePos(), 0, (struct sockaddr*) &m_remote_spec, sizeof(m_remote_spec));
	
	return iResult;
}

NetBuffer* NetConnection::GetMessage()
{
	NetBuffer* buff;
	
	InternalPlatformQueueSynchronizationEnter();
	
	if(m_receivedMessages.size() > 0)
	{
		buff = m_receivedMessages.front();
		m_receivedMessages.pop();
	}
	else
	{
		buff = NULL;
	}
	
	InternalPlatformQueueSynchronizationLeave();
	
	return buff;
}
