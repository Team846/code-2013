#ifndef AUTONOMOUS_ROUTINES_H_
#define AUTONOMOUS_ROUTINES_H_

#include "../Process/SynchronizedProcess.h"
#include "Actions/IRobotAction.h"
#include "Actions/PauseAction.h"
#include "../ComponentData/RobotData.h"
#include "../ComponentData/DrivetrainData.h"
#include "../ComponentData/AutoAimData.h"
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
class AutonomousRoutines : public AsyncProcess
{
public:
	AutonomousRoutines();
	~AutonomousRoutines();
	
	
	void TeleopTick(); //called every cycle to manage semi-autonomous functions
protected:
	virtual INT32 Tick();
private:
	void StopDrive();
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
	void SafeWait(double seconds, int safeCheckFrequency);
	
	double m_autonomousStartTime;
	
	data::ComponentData* m_componentData;
	
	AutoActions* m_autoActions;
	
	static const unsigned int standardWaitTicks = 1000/200;//sysClkRateGet() = 1000
	unsigned int GetStandardWaitTicks()
	{
		return standardWaitTicks;
	}
};

#endif
