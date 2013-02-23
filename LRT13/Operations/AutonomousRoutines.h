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
	
	void Start();
	void Stop();
	bool IsRunning();
	
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
	void FireAllFrisbees(double timeoutSeconds);
	void FrontCenter(int numFrisbeesToPickUp);
	void RearCorner(int numFrisbeesToPickUp);
	
	void AutonomousFreeCPUPause();
	void SafeGrabSem(SEM_ID sem);
	
	data::ComponentData* m_componentData;
	
	AutoActions* m_autoActions;
	
	static const unsigned int standardWaitTicks = 1000/200;//sysClkRateGet() = 1000
	unsigned int GetStandardWaitTicks()
	{
		return standardWaitTicks;
	}
	
	bool m_isRunning;
};

#endif
