#ifndef LRT_ROBOT_BASE_
#define LRT_ROBOT_BASE_

#include "WPILib.h"

enum GameState
{
	DISABLED, AUTONOMOUS, TELEOPERATED
};

class LRTRobotBase : public RobotBase
{
public:
	static bool IsRunning();
	
	LRTRobotBase();
	virtual ~LRTRobotBase();
	
	virtual void RobotInit() = 0;
	
	// Inherited from RobotBase, starts the competition
	virtual void StartCompetition();
	
	// Function to execute each pass in the loop; similar to a main function, but it gets run multiple times.
	virtual void Run();

private:
	static bool m_isRunning;
	
	bool m_robotMainOverridden;
};

#endif
