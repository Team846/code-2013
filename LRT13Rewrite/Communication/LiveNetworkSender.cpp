#include "LiveNetworkSender.h"
#include "LiveNetworkSendable.h"
#include "../Config/RobotConfig.h"
#include "../RobotState.h"
#include "../Utils/AsyncPrinter.h"

LiveNetworkSender *LiveNetworkSender::m_instance = NULL;
vector<LiveNetworkSendable*> LiveNetworkSender::liveNetworkSendables;

LiveNetworkSender* LiveNetworkSender::Instance()
{
	if (m_instance == NULL)
		m_instance = new LiveNetworkSender();
	return m_instance;
}

void LiveNetworkSender::Initialize()
{
	if (m_instance == NULL)
		m_instance = new LiveNetworkSender();
}

void LiveNetworkSender::Finalize()
{
	delete m_instance;
	m_instance = NULL;
}

LiveNetworkSender::LiveNetworkSender()
{
}

LiveNetworkSender::~LiveNetworkSender()
{
}

void LiveNetworkSender::Run()
{
	if (RobotState::Instance().GameMode() != RobotState::DISABLED)
	{
		for (vector<LiveNetworkSendable*>::iterator it = liveNetworkSendables.begin(); it < liveNetworkSendables.end(); it++)
		{
			(*it)->Send();
		}
	}
}

template void LiveNetworkSender::Send<int>(int value, string key, string table);
template void LiveNetworkSender::Send<float>(float value, string key, string table);
template void LiveNetworkSender::Send<double>(double value, string key, string table);
template<> void LiveNetworkSender::Send<bool>(bool value, string key, string table)
{
	if (tables.find(table) != tables.end())
		tables[table] = NetworkTable::GetTable(table);
	tables[table]->PutBoolean(key, value);
}

template<> void LiveNetworkSender::Send<string>(string value, string key, string table)
{
	if (tables.find(table) != tables.end())
		tables[table] = NetworkTable::GetTable(table);
	tables[table]->PutString(key, value);
}

template<typename T> void LiveNetworkSender::Send(T value, string key, string table)
{
	if (tables.find(table) != tables.end())
		tables[table] = NetworkTable::GetTable(table);
	tables[table]->PutNumber(key, (double)value);
}

void LiveNetworkSender::RegisterLiveNetworkSendable(LiveNetworkSendable* liveNetworkSendable)
{
	liveNetworkSendables.push_back(liveNetworkSendable);
}
