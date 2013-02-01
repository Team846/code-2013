#include "NetConnection.h"
#include "NetBuffer.h"

NetConnection::NetConnection(char * ip, int port, NetConnectionType connType)
{
	this->m_ip = ip;
	this->m_port = port;
	this->m_connType = connType;
}

NetConnection::~NetConnection()
{
	Close();
}

void NetConnection::Update()
{
	int received;
	char rcv_buffer[MAX_RECEIVE_BUFFER_SIZE];
	int addr_size = sizeof(m_remote_spec);
	
	while((received = recvfrom(m_socket, rcv_buffer, MAX_RECEIVE_BUFFER_SIZE, 0, (struct sockaddr*)&m_remote_spec, &addr_size)) > 0)
	{
		NetBuffer* buff = new NetBuffer(rcv_buffer, received);
		
		m_receivedMessages.push(buff);
	}
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
	
	return 0;
}

int NetConnection::Close()
{
	return close(m_socket);
}

int NetConnection::Send(NetBuffer buff)
{
	if(buff.m_sent)
	{
		return SEND_FAILED_BUFFER_ALREADY_SENT;
	}

	if(buff.m_internalBuffer == NULL)
	{
		return SEND_FAILED_BUFFER_INVALID;
	}
	
	buff.m_sent = true;
	
	int iResult = sendto(m_socket, buff.m_internalBuffer, buff.GetBytePos(), 0, (struct sockaddr*) &m_remote_spec, sizeof(m_remote_spec));
	
	return iResult;
}
