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
 * @brief Performs autonomous routine and contains autonomous functions. It has two parts, the Tick function which is meant to run every cycle to operate semi-automated processes
 * @author Raphael Chang, Tony Peng, Brian Axelrod
 */
class AutonomousRoutines 
{
public:
	AutonomousRoutines();
	~AutonomousRoutines();
	
	/*
	 * @brief Stops autonomous routine and allows routine to start again when autonomous mode is re-entered. Should be called when game state is not autonomous.
	 */
	void Stop();
	bool IsRunning();
	
	enum routines
	{
		STANDING_3FRISBEE = 1,
		STANDING_2FRISBEE = 2,
		MID_4FRISBEE = 3,
		BACK_7FRISBEE = 4,
	};
	
	void Tick(); //called every cycle to manage semi-autonomous functions
	
private:
	/*
	 * @brief Runs autonomous routine
	 */
	void Autonomous();
	
	void ServiceAutoAimBackBoard();
	void ServiceAutoAimPyramid();
	void ServiceFeederStationApproach();
	
	/********* One function per auto routine ************/
	void FireFrisbees(int numFrisbeesToFire, double timeoutSeconds);
	void FrontCenter4FrisbeeRoutine();
	void RearCornerToMidline(int numFrisbeesToPickUp);
	
	data::ComponentData* m_componentData;
	
	AutoActions* m_autoActions;
	
	bool m_isRunning;
};

#endif
