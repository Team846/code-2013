#ifndef AUTONOMOUS_ROUTINES_H_
#define AUTONOMOUS_ROUTINES_H_

#include "../Process/SynchronizedProcess.h"
#include "Actions/IRobotAction.h"
#include "Actions/PauseAction.h"
#include "../ComponentData/RobotData.h"
#include "../ComponentData/DrivetrainData.h"
#include "../ComponentData/ConfigLoaderData.h"

#include <queue>

namespace data
{
class ComponentData;
}
class AutoActions;

/*!
 * @brief Performs autonomous routine and contains autonomous functions
 * @author Raphael Chang, Tony Peng
 */
class AutonomousRoutines : public SynchronizedProcess
{
public:
	AutonomousRoutines(char * taskName, INT32 priority);
	~AutonomousRoutines();
	
	/*
	 * @brief Stops autonomous routine and allows routine to start again when autonomous mode is re-entered. Should be called when game state is not autonomous.
	 */
	void Stop();
	bool IsRunning();
	
	enum autonStage
	{
		INIT = 1,
		DRIVE_FORWARD = 2,
		AUTO_AIM = 3,
		SHOOT = 4,
		COMPLETED = 5
	};
protected:
	INT32 Tick();
private:
	/*
	 * @brief Runs autonomous routine
	 */
	void Autonomous();
	
	/*
	 * @brief Creates a queue of actions to run
	 */
	void LoadQueue();
	
	IRobotAction* m_autonActions[1];
	
	int m_autonActionCount;

	data::ComponentData* m_componentData;
	autonStage m_currentStage;
	queue<autonStage> m_routine;
	
	AutoActions* m_autoActions;
	
	bool m_isRunning;
	
	const static autonStage DRIVE_FORWARD_AND_SHOOT[5];
};

#endif
