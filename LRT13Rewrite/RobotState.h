#ifndef ROBOT_STATE_H_
#define ROBOT_STATE_H_

#include <RobotBase.h>
#include <Timer.h>

class RobotState
{
public:
	enum Mode
	{
		DISABLED = 0,
		AUTONOMOUS = 1,
		TELEOPERATED = 2
	};
	static void Initialize();
	static RobotState& Instance();
	static void Finalize();
	
	Mode GameMode();
	Mode LastGameMode();
	double GameTime(); // Seconds
	double TotalTime(); // Seconds
	
	static void Update();
private:
	RobotState();
	~RobotState();
	static RobotState *m_instance;
	
	Mode m_gameMode;
	Mode m_lastGameMode;
	Timer m_gameTimer;
	Timer m_totalTimer;
	
	DISALLOW_COPY_AND_ASSIGN(RobotState);
};

#endif
