#include "AutoAim.h"

using namespace data;
using namespace drivetrain;

AutoAim::AutoAim()
{
	m_client = new NetClient();
	m_client->Open();
	
	sockaddr_in endpoint;
	endpoint.sin_family = AF_INET;
	endpoint.sin_addr.s_addr = inet_addr("10.8.46.8");
	endpoint.sin_port = htons(8000);
	
	m_client->Connect(endpoint);
}

AutoAim::~AutoAim()
{
	DELETE(m_client);
}

void AutoAim::Configure()
{
	m_maxError = m_config->Get<int>("autoaim", "max_error", 5);
}

void AutoAim::Update()
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
		
		if(desiredX - x < -m_maxError)
		{
			// we're to the left
			ComponentData::GetInstance()->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
			ComponentData::GetInstance()->drivetrainData->setRelativePositionSetpoint(TURN, -5, 0.3);
			ComponentData::GetInstance()->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 2));
			
			NetBuffer buff;
						
			buff.Write((char)MessageType::AUTO_AIM_DATA);
			buff.Write(strength);
			buff.Write(false);
			
			SmarterDashboard::Instance()->EnqueueMessage(&buff, NetChannel::NET_UNRELIABLE_SEQUENCED, 2);
		}
		else if(desiredX - x > m_maxError)
		{
			// we're to the right
			ComponentData::GetInstance()->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
			ComponentData::GetInstance()->drivetrainData->setRelativePositionSetpoint(TURN, 5, 0.3);
			ComponentData::GetInstance()->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 2));
			
			NetBuffer buff;
						
			buff.Write((char)MessageType::AUTO_AIM_DATA);
			buff.Write(strength);
			buff.Write(false);
			
			SmarterDashboard::Instance()->EnqueueMessage(&buff, NetChannel::NET_UNRELIABLE_SEQUENCED, 2);
		}
		else
		{
			// we're lined up!
			NetBuffer buff;
			
			buff.Write((char)MessageType::AUTO_AIM_DATA);
			buff.Write(strength);
			buff.Write(true);
			
			SmarterDashboard::Instance()->EnqueueMessage(&buff, NetChannel::NET_UNRELIABLE_SEQUENCED, 2);
		}
		
		DELETE(msg);
	}
}
