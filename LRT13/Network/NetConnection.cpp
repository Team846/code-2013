#include "NetBuffer.h"
#include "NetConnection.h"

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
		
		// synchronize on the semaphore so that we make sure we're safely accessing the internal message queue
		InternalPlatformQueueSynchronizationEnter();
		m_receivedMessages.push(buff);
		InternalPlatformQueueSynchronizationLeave(); // release the lock on the queue
	}
}

void NetConnection::InternalPlatformQueueSynchronizationLeave()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semGive(m_msgQueueMutex);
#endif
}

void NetConnection::InternalPlatformCreateUpdateTask()
{
	static int counter = 0;
	
	stringstream s;
	
	s << "NetConnection" << counter;
	
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalUpdateTask = new Task(s.str().c_str(), (FUNCPTR)InternalPlatformUpdateTaskWrapper);
#endif
}

void NetConnection::InternalPlatformRunUpdateTask()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalUpdateTask->Start((UINT32)this);
#endif
}

void NetConnection::InternalPlatformDestroyUpdateTask()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalUpdateTask->Stop();
	
	DELETE(m_internalUpdateTask);
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
	
	InternalPlatformCreateUpdateTask();
	InternalPlatformRunUpdateTask();
	
	return 0;
}

int NetConnection::Close()
{
	m_isRunning = false;
	
#warning this function is potentially bad because we might be killing the thread while it's waiting for a message. -tp
	InternalPlatformDestroyUpdateTask();
	
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
	
	int netchannel = method + channel;
	
	MessageAwaitingACK maack;
	
	maack.buff = &buff;
	maack.sentTime = Timer::GetFPGATimestamp();
	
	NetBuffer* localBuff = new NetBuffer();
	localBuff->Write((char)USER_DATA);
	localBuff->Write((char)method);
	
	switch(method)
	{
		// TODO: specific handlers for different methods
	case NetChannel::NET_RELIABLE:
		localBuff->Write(m_currentReliableUnorderedCounter);
		m_reliableUnordered[channel][m_currentReliableUnorderedCounter++] = maack;
		break;
	case NetChannel::NET_RELIABLE_IN_ORDER:
		localBuff->Write(m_currentReliableOrderedCounter);
		m_reliableOrdered[channel][m_currentReliableOrderedCounter++] = maack;
		break;
	case NetChannel::NET_RELIABLE_SEQUENCED:
		localBuff->Write(m_currentReliableSequencedCounter);
		m_reliableSequenced[channel][m_currentReliableSequencedCounter++] = maack;
		break;
	case NetChannel::NET_UNRELIABLE:
		 break;
	case NetChannel::NET_UNRELIABLE_SEQUENCED:
		localBuff->Write(m_currentUnreliableSequencedCounter++);
		break;
	default:

		break;
	}
	
	//TODO: implement reliable UDP
	//NetBuffer* localBuff = new NetBuffer();

	//localBuff->Write(buff.m_internalBuffer);
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
