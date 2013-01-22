#include "RobotData.h"

using namespace data;

int RobotData::_id = -1;
RobotData::RobotState RobotData::m_state = RobotData::DISABLED;
vector<RobotData::Data> RobotData::m_loggedClasses;

int RobotData::GetMissedPacketsInLifetime() { return 0; }
void RobotData::IncrementMissedPacketsInLifetime() { return; }

int RobotData::GetCycleCount() { return 0; }
void RobotData::IncrementCycleCount() { return; }

RobotData::RobotState RobotData::GetCurrentState() { return m_state; }
void RobotData::SetRobotState(RobotData::RobotState state) { m_state = state; }

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

#warning Check me
string RobotData::Serialize()
{
	CompleteMessage cm;
	
	for(vector<Data>::iterator it = m_loggedClasses.begin(); it != m_loggedClasses.end(); ++it)
	{
		Data d = *it;
		
		// assert that the two maps are synchronized
		if(d.indexToTypeMap.size() != d.indexToValueMap.size())
		{
			// raise error
			
			return "-1";
		}
		
		DataMessage* dMsg = cm.add_data();
		
		dMsg->set_classname(d.className);
		
		for(int i = 0; i < d.indexToTypeMap.size(); i++)
		{
			dMsg->mutable_indextotypemap()->add_type(d.indexToTypeMap[i]);
			dMsg->mutable_indextovaluemap()->add_value(d.indexToValueMap[i]);
		}
	}
	
	return cm.SerializeAsString();
}
