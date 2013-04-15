#include "AutoAim.h"

using namespace data;
using namespace drivetrain;

AutoAim::AutoAim()
: AsyncProcess("AutoAim")
{
	m_client = new NetClient();
	m_client->Open();
	
	sockaddr_in endpoint;
	endpoint.sin_family = AF_INET;
	endpoint.sin_addr.s_addr = inet_addr("10.8.46.8");
	endpoint.sin_port = htons(8000);
	
	m_client->Connect(endpoint);
	
	m_componentData = ComponentData::GetInstance();
}

AutoAim::~AutoAim()
{
	DELETE(m_client);
}

void AutoAim::Configure()
{
	m_maxError = m_config->Get<int>("autoaim", "max_error", 5); // Error each direction
	m_desiredX = m_config->Get<int>("autoaim", "desired_x", 197);
	m_desiredY = m_config->Get<int>("autoaim", "desired_y", 80);
}

INT32 AutoAim::Tick()
{
	NetBuffer* msg;
	
	while((msg = m_client->ReadMessage()) != NULL)
	{
		// verify that this is what we want
		MessageType::Enum msgtype = (MessageType::Enum)msg->ReadChar();
		
		if(msgtype != MessageType::AUTO_AIM_DATA)
		{
			AsyncPrinter::Printf("[WARNING] AutoAim::Update(): Invalid packet received with header %d.", msgtype);
		}
		
		int x = msg->ReadInt32();
		int y = msg->ReadInt32();
		
		int desiredX = msg->ReadInt32();
		int desiredY = msg->ReadInt32();
		
		double strength = msg->ReadDouble();
		
		AsyncPrinter::Printf("AutoAim: %d %d\n", x, y);
		m_componentData->autoAimData->setCurrentX(x);
		m_componentData->autoAimData->setCurrentY(y);
		m_componentData->autoAimData->setDesiredX(desiredX);
		m_componentData->autoAimData->setDesiredY(desiredY);
		m_componentData->autoAimData->setStrength(strength);
		
		NetBuffer buff;
					
		buff.Write((char)MessageType::AUTO_AIM_DATA);
		buff.Write(strength);
		buff.Write(true);
		
		SmarterDashboard::Instance()->EnqueueMessage(&buff, NetChannel::NET_UNRELIABLE_SEQUENCED, 2);
					
		DELETE(msg);
	}
	
	return 0;
}
