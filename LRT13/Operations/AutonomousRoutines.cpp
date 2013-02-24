#include "AutonomousRoutines.h"
#include "../ComponentData/ComponentData.h"
#include "../ComponentData/ShooterData.h"
#include "AutoActions.h"
#include "../Config/DriverStationConfig.h"
#include "../Utils/AsyncPrinter.h"

using namespace data;
using namespace data::drivetrain;
using namespace data::shooter;
//unsigned int AutonomousRoutines::standardWaitTicks = ;

AutonomousRoutines::AutonomousRoutines()
{
	m_componentData = ComponentData::GetInstance();
	
	m_autoActions = new AutoActions();
	
	m_isRunning = false;
}

AutonomousRoutines::~AutonomousRoutines()
{

}

void AutonomousRoutines::Tick()
{
	switch (m_autoActions->GetCurrentAutonomousAction())
	{
	case AutoActions::AUTO_AIM_BACKBOARD:
		ServiceAutoAimBackBoard();
		break;
	case AutoActions::AUTO_AIM_PYRAMID:
		ServiceAutoAimPyramid();
		break;
	case AutoActions::FEEDER_STATION_APPROACH:
		ServiceFeederStationApproach();
		break;
	case AutoActions::NO_ADV_AUTOMATION:
		//ne faire rien
		break;
	}
}

void AutonomousRoutines::Autonomous()
{
}

void AutonomousRoutines::ServiceAutoAimBackBoard()
{
	//TODO writeme
}

void AutonomousRoutines::ServiceAutoAimPyramid()
{
	//TODO writeme
}

void AutonomousRoutines::ServiceFeederStationApproach()
{
	//TODO writeme
}

/********* One function per auto routine ************/
void AutonomousRoutines::FireAllFrisbees(double timeoutSeconds)
{
	UINT32 startTime = GetFPGATime();//Microseconds
	while (m_isRunning && GetFPGATime() < startTime + timeoutSeconds * 1E6 && m_componentData->shooterData->GetNumFrisbeesInStorage() > 0)
	{
		m_componentData->shooterData->SetShooterSetting(AUTO);
		AutonomousFreeCPUPause();
	}
	
	if (m_componentData->shooterData->GetNumFrisbeesInStorage() > 0 )
		AsyncPrinter::Printf("Shooting timed out, we did not fire all the frisbees\n");
}

void AutonomousRoutines::SafeGrabSem(SEM_ID sem)
{
	while (m_isRunning && semTake(sem, GetStandardWaitTicks()))
	{
		//ne faire rien pendant que nous attendons 
	}
}

//This is a sample outline of a routine
void AutonomousRoutines::FrontCenter(int numFrisbeesToPickUp)
{
/*write worst case start and end time of the op here*/
/*0-2*/	FireAllFrisbees(3.0);
/*2-2*/ //start driving back a distance. Distance depends on # of frisbees
/*2-2*/ //start deploying the collector
/*2-8*/ //Wait until right # of frisbees picked up or  distance is traveleld
/*8-10*/ FireAllFrisbees(3.0);
/*10-10*/ //start driving back to pick up more frisbees if there is a need
/*10-13*/ //Wait until right # of frisbees picked up or  distance is traveleld
/*13-15*/ //Fire rest of frisbees
	//TODO edit me once we know more about the robot
}

void AutonomousRoutines::RearCorner(int numFrisbeesToPickUp)
{
	//TODO writeme
}

void AutonomousRoutines::Stop()
{
	m_isRunning = false; 
}

void AutonomousRoutines::Start()
{
	m_isRunning = true; 
	//faire qqch d'autre dans l'avenir
}

bool AutonomousRoutines::IsRunning()
{
	return m_isRunning;
}

void AutonomousRoutines::AutonomousFreeCPUPause()
{
	taskDelay(GetStandardWaitTicks());//4x per cycle. We don't really do anything computationall intensive in these function
}
