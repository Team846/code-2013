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
:  AsyncProcess("AutonTask")
{
	m_componentData = ComponentData::GetInstance();
	
	m_autoActions = new AutoActions();
	
	m_isRunning = false;
}

AutonomousRoutines::~AutonomousRoutines()
{

}

void AutonomousRoutines::TeleopTick()
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
	double delay = DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_DELAY);
	delay *= 3;//factor so that we can wait the whole auton
	//waiting the delay
	AsyncPrinter::Printf("STarting auton\n");
	SafeWait(delay, 10);

	printf("Wait is done\n");
	
	int routine = (int) (DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_SELECT) + 0.5);
	AsyncPrinter::Printf("STarting routine number %d\n", routine);
	
	switch (routine)
	{
	//simple shoot 2 routine
	case 0:
		m_componentData->shooterData->SetNumFrisbeesInStorage(2);
		FireAllFrisbees(15.0);
		break;
	//simple shoot 3 routine
	case 1:
		m_componentData->shooterData->SetNumFrisbeesInStorage(3);
		FireAllFrisbees(15.0);
		break;
		
	//simple shoot 10 routine to make sure we unload all of our frisbees
	case 2:
		m_componentData->shooterData->SetNumFrisbeesInStorage(10);
		FireAllFrisbees(15.0);
		break;
		
	case 3://start in back and do the fancy driving back routine
		break;
		
	case 4://test routine
		break;
		
	}
	//initial wait
	AsyncPrinter::Printf("Auton done\n");
	m_isRunning = false;
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
	m_isRunning = true;
	UINT32 startTime = GetFPGATime();//Microseconds
	AsyncPrinter::Printf("is running %d\n", m_isRunning);
	AsyncPrinter::Printf("time: %d, endtime %d\n", GetFPGATime(), startTime + timeoutSeconds);
	
	AsyncPrinter::Printf("numFrisees %d\n", m_componentData->shooterData->GetNumFrisbeesInStorage() );
	while (m_isRunning && GetFPGATime() < startTime + timeoutSeconds * 1E6 && m_componentData->shooterData->GetNumFrisbeesInStorage() > 0)
	{
		AsyncPrinter::Printf("Firing frisbees\n");
		m_componentData->shooterData->SetShooterSetting(CONTINOUS);
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

void AutonomousRoutines::SafeWait(double seconds, int safeCheckFrequency)
{
	taskDelay((UINT32) (sysClkRateGet() * seconds));
	return;
	AsyncPrinter::Printf("Waiting for %.2f td\n", seconds);
//	Wait(seconds);
	AsyncPrinter::Printf("Just Waited for %.2f w/ taskdelay\n", seconds);
	taskDelay((UINT32) (sysClkRateGet()));
	AsyncPrinter::Printf("Just Waited for another second w/ taskdelay\n");
	return;
	
	if (sysClkRateGet()  != 1000)
	{
		AsyncPrinter::Printf("One of the auton wait assuptions does not hold ture and thus might not work\n");
	}
	UINT32 microsecondsToWait = (UINT32) (seconds * 1E6);
	UINT32 currTime = GetFPGATime();
	if (microsecondsToWait + currTime < currTime)//we have an overflow
	{
		AsyncPrinter::Printf("Auton delay causes an overflow! D:\n We thought this would never happen so we didn't write code to handle it. It should never happen.");
	}
	
	UINT32 endTime = currTime + microsecondsToWait;
	int unitWaitTicks = sysClkRateGet() / safeCheckFrequency;
	int unitWaitMicroseconds = 1000 * unitWaitTicks;
	
	UINT32 timeRemaining; 
	while (m_isRunning && endTime - GetFPGATime() > 0)
	{
		timeRemaining = endTime - GetFPGATime();
		if (timeRemaining > unitWaitMicroseconds)
			taskDelay(unitWaitTicks);
		else
		{
			taskDelay(timeRemaining / 1E3 );// /1E6 and then * sysClkRateGet =1e3
			break;
		}
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


void AutonomousRoutines::AutonomousFreeCPUPause()
{
	taskDelay(GetStandardWaitTicks());//4x per cycle. We don't really do anything computationall intensive in these function
}

INT32 AutonomousRoutines::Tick()
{
	Autonomous();
}
