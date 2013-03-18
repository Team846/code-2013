#include "RobotData.h"

using namespace data;

int RobotData::_id = -1;
int RobotData::m_frisbees = 0;

RobotData::RobotState RobotData::m_state = RobotData::DISABLED;
RobotData::RobotState RobotData::m_laststate = RobotData::DISABLED;
vector<RobotData::Data> RobotData::m_loggedClasses;

vector<RobotData::DataPacket> RobotData::m_frameList;
vector<vector<RobotData::DataPacket> > RobotData::m_lifetimeList;

int RobotData::GetMissedPacketsInLifetime() { return 0; }
void RobotData::IncrementMissedPacketsInLifetime() { return; }

int RobotData::GetCycleCount() { return 0; }
void RobotData::IncrementCycleCount() { return; }

RobotData::RobotState RobotData::GetCurrentState()
{
	return m_state;
}

RobotData::RobotState RobotData::GetLastState()
{
	return m_laststate;
}

void RobotData::SetRobotState(RobotData::RobotState state)
{
	m_laststate = m_state;
	m_state = state;
}

int RobotData::AllocateKey(string className)
{
	int id = ++_id;
	
	// assert that this id will be the next one in the vector
	if(id != m_loggedClasses.size())
	{
		// raise error
		return -1;
	}
	
	Data d;
	
	d.className = className;
	d.indexToTypeMap;
	d.indexToValueMap;
	
	return id; 
}

void RobotData::AddValue(int key, string type, string serialized)
{
	if(key >= m_loggedClasses.size())
	{
		// raise error
	}
	
	// assert that the two maps are synchronized
	if(m_loggedClasses[key].indexToTypeMap.size() != m_loggedClasses[key].indexToValueMap.size())
	{
		// raise error
		
		return;
	}
	
	m_loggedClasses[key].indexToTypeMap.push_back(type);
	m_loggedClasses[key].indexToValueMap.push_back(serialized);
}


void RobotData::Serialize()
{
	for(vector<DataPacket>::iterator it = m_frameList.begin(); it != m_frameList.end(); it++)
	{
		(*it).netConn.Send((*it).netBuff,(*it).channelType,(*it).channel );
	}
}

void RobotData::IncrementFrisbeeCounter()
{
	m_frisbees++;
}

void RobotData::DecrementFrisbeeCounter()
{
	m_frisbees--;
}

int RobotData::GetFrisbeeCounter()
{
	return m_frisbees;
}

void RobotData::NewFrame()
{
	m_lifetimeList.push_back(m_frameList);
	m_frameList.clear();
}

void RobotData::EnqueueBuffer(NetBuffer buff, NetConnection netConn, NetChannel::Enum channelType, int channel )
{
	DataPacket packet;
	packet.netBuff = buff;
	packet.netConn = netConn;
	packet.channel = channel;
	packet.channelType = channelType;
	
	m_frameList.push_back(packet);
}

