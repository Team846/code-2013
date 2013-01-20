#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#warning proposed code

#include <string>
#include <vector>

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
		
		static int GetMissedPacketsInLifetime();
		static void IncrementMissedPacketsInLifetime();
		
		static int GetCycleCount();
		static void IncrementCycleCount();
		
		static RobotState GetCurrentState();
		static void SetRobotState(RobotState state);
		
		static int AllocateKey();
		static void AddValue(int key, string type, string serialized);
		
	private:
		static int m_missedPacketsInLifetime;
		static int m_cycleCount;

		static RobotState m_state;
		
#warning rename/refactor me
		struct Data
		{
			string className;
			vector<string> indexToTypeMap;
			vector<string> indexToValueNameMap;
		};
		
		static vector<Data> m_loggedClasses;
	};
};

#endif
