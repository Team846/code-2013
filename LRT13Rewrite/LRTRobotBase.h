#ifndef LRT_ROBOT_BASE_
#define LRT_ROBOT_BASE_

#include <WPILib.h>
#include <semLib.h>

class LRTRobotBase : public RobotBase
{
public:
	LRTRobotBase();
	virtual ~LRTRobotBase();
	
	virtual void RobotInit() = 0;
	
	// Inherited from RobotBase, starts the competition
	virtual void StartCompetition();
	
	// Function to execute each pass in the loop; similar to a main function, but it gets run multiple times.
	virtual void Main();

private:
	void Run();

	Notifier m_loopSynchronizer;
	static bool m_isRunning;

	static void ReleaseLoop(void * param);
	
	SEM_ID m_loopSynchSem;
	bool m_robotMainOverridden;
};

#endif
