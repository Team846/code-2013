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

	m_telemetryTypeNames[0] = "Climber State";
	m_telemetryTypeNames[1] = "Winch Pawl Current";
	m_telemetryTypeNames[2] = "Winch Pawl Duty Cycle";
	m_telemetryTypeNames[3] = "Winch Pawl Status";
	
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
	static int counter = 0;
	
	if(counter++ % 50 == 0)
		AsyncPrinter::Printf("SmarterDashboard::Tick(): %d\n", counter);
	
	// perhaps later, we may want to send some heartbeats or packets of the sort
	Flush();
	
	// clear out the telemetry
	for(int i = TelemetryType::BEGIN; i < TelemetryType::END; i++)
	{
		m_telemetryData[i] = "???";
	}
}

void SmarterDashboard::Flush()
{
	//AsyncPrinter::Printf("Flushing queue...messages: %d\n", m_netBufferQueue.size());
	{
		Synchronized s(m_queueSem);
		
		for(int i = 0; i < m_netBufferQueue.size(); i++)
		{
			SD_Message nb = m_netBufferQueue.front();
			m_netBufferQueue.pop();
			
			m_server->SendToAll(nb.nb, nb.method, nb.channel);
			
			AsyncPrinter::Printf("Sending message...\n");
		}
	} // m_queueSem
}

template void SmarterDashboard::SetTelemetryData<double>(TelemetryType::Enum type, double data);
template void SmarterDashboard::SetTelemetryData<string>(TelemetryType::Enum type, string data);
template void SmarterDashboard::SetTelemetryData<float>(TelemetryType::Enum type, float data);

template<class T> void SmarterDashboard::SetTelemetryData(TelemetryType::Enum type, T data)
{
	stringstream ss;
	ss << std::boolalpha << data;
	
	m_telemetryData[type] = ss.str();
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

void SmarterDashboard::EnqueueShooterMessage(MessageType::Enum header, float time, float value)
{
	NetBuffer buff;
	
	buff.Write((char)header);
	buff.Write(time);
	buff.Write(value);
	
	AsyncPrinter::Printf("Enqueueing shooter message.\n");
	
	EnqueueMessage(&buff, NetChannel::NET_UNRELIABLE_SEQUENCED, 1);
}
