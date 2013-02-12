#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#include <string>
#include <vector>

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
		
		enum Orientation
		{
			UP = 0,
			DOWN = 1
		};
		static void IncrementFrisbeeCounter(Orientation);
		static void DecrementFrisbeeCounter(Orientation);
		static int GetFrisbeeCounter();
		static int GetFrisbeeCounter(Orientation);
		
		static int GetMissedPacketsInLifetime();
		static void IncrementMissedPacketsInLifetime();
		
		static int GetCycleCount();
		static void IncrementCycleCount();
		
		static RobotState GetCurrentState();
		static void SetRobotState(RobotState state);
		
		static int AllocateKey(string className);
		static void AddValue(int key, string type, string serialized);
		
		static string Serialize();
		
	private:
		static int m_missedPacketsInLifetime;

		static RobotState m_state;
		static int _id;
		static int m_frisbees[2];
		
#warning rename/refactor me
		struct Data
		{
			string className;
			vector<string> indexToTypeMap;
			vector<string> indexToValueMap;
		};
		
		static vector<Data> m_loggedClasses;
	};
};

#endif
