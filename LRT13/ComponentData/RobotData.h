#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#include <string>
#include <vector>

#include "../Network/NetConnection.h"
#include "../Network/NetBuffer.h"

//#include "robotdata.pb.h"

using namespace std;

namespace data
{
	/*!
	 * @brief Provides an area to store general data or statistics collected during runtime.
	 * @author Tony Peng
	 */
	class RobotData
	{
	public:
		enum RobotState
		{
			AUTONOMOUS = 0,
			TELEOP = 1,
			DISABLED = 2
		};
		
		static void IncrementFrisbeeCounter();
		static void DecrementFrisbeeCounter();
		static int GetFrisbeeCounter();
		
		static int GetMissedPacketsInLifetime();
		static void IncrementMissedPacketsInLifetime();
		
		static int GetCycleCount();
		static void IncrementCycleCount();
		
		static RobotState GetCurrentState();
		static void SetRobotState(RobotState state);
		
		static int AllocateKey(string className);
		static void AddValue(int key, string type, string serialized);
		
		static void Serialize();
		
		static void NewFrame();
		static void EnqueueBuffer(NetBuffer buff, NetConnection netConn, NetChannel::Enum channelType, int channel);
		
		
	private:
		
		static int m_missedPacketsInLifetime;

		static RobotState m_state;
		static int _id;
		static int m_frisbees;
		
		
#warning rename/refactor me
		struct Data
		{
			string className;
			vector<string> indexToTypeMap;
			vector<string> indexToValueMap;
		};
		
		struct DataPacket
		{
			DataPacket();
			NetBuffer netBuff;
			NetConnection netConn;
			int channel;
			NetChannel::Enum channelType;
		};
		
		static vector<Data> m_loggedClasses;
		
		static vector<DataPacket> m_frameList;
		static vector<vector<DataPacket> > m_lifetimeList;
	};
};

#endif
