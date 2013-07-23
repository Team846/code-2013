#include "AutoAim.h"

using namespace data;
using namespace drivetrain;

AutoAim::AutoAim()
: SynchronizedProcess("AutoAim", Task::kDefaultPriority + 2)
{
	m_server = new NetServer(8000);
	m_server->Open();
	
	m_componentData = ComponentData::GetInstance();
}

AutoAim::~AutoAim()
{
	DELETE(m_server);
}

void AutoAim::Configure()
{
	m_maxError = m_config->Get<int>("autoaim", "max_error", 5); // Error each direction
	m_desiredX = m_config->Get<int>("autoaim", "desired_x", 197);
	m_desiredY = m_config->Get<int>("autoaim", "desired_y", 80);
}

INT32 AutoAim::Tick()
{
	m_componentData->autoAimData->setDesiredX(m_desiredX);
	m_componentData->autoAimData->setDesiredY(m_desiredY);
	m_componentData->autoAimData->setErrorThreshold(m_maxError);
	
	NetBuffer* msg;
	
	while((msg = m_server->ReadMessage()) != NULL)
	{
		// verify that this is what we want
		MessageType::Enum msgtype = (MessageType::Enum)msg->ReadChar();
		
		if(msgtype != MessageType::AUTO_AIM_DATA)
		{
			AsyncPrinter::Printf("[WARNING] AutoAim::Update(): Invalid packet received with header %d.", msgtype);
		}
		
		int x = msg->ReadInt32();
		int y = msg->ReadInt32();
		
		double strength = msg->ReadDouble();
		
//		AsyncPrinter::Printf("AutoAim: %d %d\n", x, y);
		m_componentData->autoAimData->setCurrentX(x);
		m_componentData->autoAimData->setCurrentY(y);
		m_componentData->autoAimData->setStrength(strength);
		
		NetBuffer buff;
					
		buff.Write((UINT8)MessageType::AUTO_AIM_DATA);
		buff.Write(strength);
		buff.Write(true);
		
		SmarterDashboard::Instance()->EnqueueMessage(&buff, NetChannel::NET_UNRELIABLE_SEQUENCED, 2);
					
		DELETE(msg);
	}
	
	taskDelay(sysClkRateGet() / 100);
	
	return 0;
}
