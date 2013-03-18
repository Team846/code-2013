#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#include <string>
#include <vector>

#include "../Network/NetPeer.h"
#include "../Network/NetConnectionType.h"
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
		static RobotState GetLastState();
		static void SetRobotState(RobotState state);
		
		static int AllocateKey(string className);
		static void AddValue(int key, string type, string serialized);
		
		static void Serialize();
		
		static void NewFrame();
		static void EnqueueBuffer(NetBuffer buff, NetPeer netConn, NetChannel::Enum channelType, int channel);
		
		
	private:
		
		static int m_missedPacketsInLifetime;

		static RobotState m_state;
		static RobotState m_laststate;
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
			DataPacket() : netConn("10.8.46.5", 80, Network::SERVER){};//Finish writing your code before you commit it, seriously, don't give me stuff with linker errors. I changed this to make it compile. FIx it so it actually works. 
			NetBuffer netBuff;
			NetPeer netConn;
			int channel;
			NetChannel::Enum channelType;
		};
		
		static vector<Data> m_loggedClasses;
		
		static vector<DataPacket> m_frameList;
		static vector<vector<DataPacket> > m_lifetimeList;
	};
};

#endif
