#include "LiveNetworkSendable.h"

LiveNetworkSendable::LiveNetworkSendable()
{
	m_sender = LiveNetworkSender::Instance();
	LiveNetworkSender::RegisterLiveNetworkSendable(this);
}

LiveNetworkSendable::~LiveNetworkSendable()
{
	
}
