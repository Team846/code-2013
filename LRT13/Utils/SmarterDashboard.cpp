#include "SmarterDashboard.h"

SmarterDashboard* SmarterDashboard::m_instance = NULL;

SmarterDashboard* SmarterDashboard::Instance()
{
	if(m_instance == NULL)
		AsyncPrinter::Printf("[ERROR] SmarterDashboard::Instance(): Start was not called!  Probably going to crash and burn now...\n");
	
	return m_instance;
}

void SmarterDashboard::Start()
{
	if(m_instance != NULL)
	{
		AsyncPrinter::Printf("[WARNING] SmarterDashboard has already been started!\n");
		return;
	}
	
	m_instance = new SmarterDashboard();
}

void SmarterDashboard::Close()
{
	if(m_instance == NULL)
	{
		AsyncPrinter::Printf("[WARNING] No instance of SmarterDashboard to close.");
		return;
	}
	
	DELETE(m_instance);
}

SmarterDashboard::SmarterDashboard()
{
	m_queueSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	
	m_server = new NetServer(1140);
	m_server->Open();
	AsyncPrinter::Println("The SmarterDashboard service is running on port 1140.");
}

SmarterDashboard::~SmarterDashboard()
{
	m_server->Close();
	DELETE(m_server);
	
	semDelete(m_queueSem);
}

void SmarterDashboard::Tick()
{
	// perhaps later, we may want to send some heartbeats or packets of the sort
	Flush();
}

void SmarterDashboard::Flush()
{
	{
		Synchronized s(m_queueSem);
		
		for(int i = 0; i < m_netBufferQueue.size(); i++)
		{
			SD_Message nb = m_netBufferQueue.front();
			m_netBufferQueue.pop();
			
			m_server->SendToAll(nb.nb, nb.method, nb.channel);
		}
	} // m_queueSem
}

void SmarterDashboard::EnqueueMessage(NetBuffer* buff, NetChannel::Enum method, int channel)
{
	SD_Message msg;
	msg.nb = buff;
	msg.method = method;
	msg.channel = channel;
			
	{
		Synchronized s(m_queueSem);
	
		m_netBufferQueue.push(msg);
	}
}
