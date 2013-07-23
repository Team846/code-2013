#include "AutonomousRoutines.h"
#include "../ComponentData/ComponentData.h"
#include "../ComponentData/CollectorData.h"
#include "../ComponentData/ShooterData.h"
#include "AutoActions.h"
#include "../Config/DriverStationConfig.h"
#include "../Utils/AsyncPrinter.h"
#include <fstream>

using namespace data;
using namespace data::drivetrain;
using namespace data::shooter;
//unsigned int AutonomousRoutines::standardWaitTicks = ;

#define FULL_AUTON 0
#define USE_COLLECTOR 1

AutonomousRoutines::AutonomousRoutines()
:  AsyncProcess("AutonTask", Task::kDefaultPriority - 1)
{
	m_componentData = ComponentData::GetInstance();
	
	m_autoActions = AutoActions::Instance();
	
	m_isRunning = false;
	
	m_autonomousStartTime = 0.0;
	
	loopSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
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
	m_autonomousStartTime = Timer::GetFPGATimestamp();
	
	m_componentData->shooterData->SetLauncherAngleLow();
	m_componentData->shooterData->SetEnabled(true);
	m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
	m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
	double delay = DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_DELAY);
	
			
	double dNumFrisbees = DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::NUM_FRISBEES) * 2;
	int numFrisbees = (int) (dNumFrisbees + 0.5);
	
	delay *= 3;//factor so that we can wait the whole auton
	//waiting the delay
	AsyncPrinter::Printf("Starting auton.\n");
	SafeWait(delay, 10);

	printf("Wait is done\n");
	
	int routine = (int) (DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_SELECT) + 0.5);
	AsyncPrinter::Printf("Starting routine number %d.\n", routine);
	
	double driveBackDistance = 0;
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
		AsyncPrinter::Printf("Crazy\n");
		m_componentData->shooterData->SetNumFrisbeesInStorage(10);
		FireAllFrisbees(15.0);
		break;
	case 3://start in back and do the fancy driving back routine
		m_componentData->shooterData->SetNumFrisbeesInStorage(6);
		FireAllFrisbees(7.0);
		m_componentData->shooterData->SetShooterSetting(OFF);
//		FireAllFrisbees(6.0);
		
		AsyncPrinter::Printf("Starting driving back\n");
		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12 * 2.6 - 1.5 - 4 , 0.90);
		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
//		StopDrive();
		
		AsyncPrinter::Printf("Starting first turn\n");
		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -35, 0.8);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 8));

		AsyncPrinter::Printf("Starting diagonal backing up\n");
////		StopDrive();
		
		
		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 92.0 + 12.0 + 7.0, 0.90);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 45));
		m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.5);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));

		SafeWait(0.3, 10);
		AsyncPrinter::Printf("Starting second turn\n");
#if USE_COLLECTOR
		m_componentData->collectorData->SlideDown();
		m_componentData->collectorData->RunRollers();
#endif
		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, 35 + 90 + 8 - 3.0 + 1, 0.8);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 6));
		SafeWait(0.3, 10);
 
		driveBackDistance = 94;
		AsyncPrinter::Printf("Starting straight run\n");
		if (numFrisbees % 2 == 1)//odd, wtf
		{
			driveBackDistance += 11.0/2;
			driveBackDistance += 11.0 * (numFrisbees - 1) / 2.0;
		}
		else
		{
			driveBackDistance += 11.0 * (numFrisbees) / 2.0;
		}
		
		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, driveBackDistance, 0.9);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, numFrisbees * 11.0 + 20));
		m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.30);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
		//we want to slow down at driveBackDistance - numFrisbees * 11.0 - 20
		AsyncPrinter::Printf("driving aligning again\n");
		
		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -driveBackDistance + 40 + 40.0 + 34 - 15 + 1.0 - 6, 0.9);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 45));
		m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.5);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
		
			
		AsyncPrinter::Printf("turning\n");
		
#if USE_COLLECTOR
		m_componentData->collectorData->SlideUp();
		m_componentData->collectorData->StopRollers();
#endif
		
		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -91.5, 0.8);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
		SafeWait(0.3, 10);
		AsyncPrinter::Printf("going back to starting pos\n");
		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -5 * 12.0 - 54 + 11, 0.9);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 50));
		m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.5);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
		//so the distance to the center is 12.0 * 8.0 - 2.0 = 94, 77 to center
//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 8.0 - 2.0, 0.90);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
//		
		AsyncPrinter::Printf("done with\n");
//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 8, 0.55);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
//		StopDrive();
//
//		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, 143.5, 0.4);
//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 6.5));
//		StopDrive();
//		
//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 10, 0.3);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 12 * 7));
//		m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.121);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.255));
//		StopDrive();
//		
//		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -45.5, 0.4);
//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 8));
//		StopDrive();
////
//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -58, 0.4);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
//		StopDrive();
//
//		m_componentData->collectorData->SlideUp();
//		m_componentData->collectorData->StopRollers();
//				
//		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -38, 0.4);
//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 8));
//		StopDrive();
//		m_componentData->shooterData->SetLauncherAngleLow();
//
//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -54 - 12 * 2.5, 0.4);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
//		AsyncPrinter::Printf("Just moved the rollers up.\n");

#if FULL_AUTON
		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -55, 0.2);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
		StopDrive();
		
		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -72, 0.2);
		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
		StopDrive();
		
		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -(90-55), 0.2);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
		StopDrive();

		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -38, 0.2);
		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
		StopDrive();
//		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		
		//-108.5
		//-2405.8166
		
		//2297.3
		//229.73 per rev
		
		//New code
		//Positive = clockwise
			
//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 4.0, 0.2);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
//		StopDrive();
//		
//		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -135, 0.2);
//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
//		StopDrive();
//		
//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 6.0, 0.2);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
//		StopDrive();
//		
		
#endif
		
		
		break;
	case 4://start in back and do the fancy driving back routine
			m_componentData->shooterData->SetNumFrisbeesInStorage(4);
			FireAllFrisbees(7.0);
	//		FireAllFrisbees(6.0);
			
			AsyncPrinter::Printf("Starting driving back\n");
			m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
			m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
			m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12 * 2.6 - 1.5 - 4 , 0.90);
			m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
	//		StopDrive();
			
			AsyncPrinter::Printf("Starting first turn\n");
			m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
			m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -35, 0.8);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 8));

			AsyncPrinter::Printf("Starting diagonal backing up\n");
	////		StopDrive();
			
			
			m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 92.0 + 12.0 + 7.0, 0.90);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 45));
			m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.5);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));

			SafeWait(0.2, 10);
			AsyncPrinter::Printf("Starting second turn\n");
	#if USE_COLLECTOR
			m_componentData->collectorData->SlideDown();
			m_componentData->collectorData->RunRollers();
	#endif
			m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, 35 + 90 + 8 - 3.0 + 1, 0.8);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 6));
			SafeWait(0.2, 10);
	 
			driveBackDistance = 94;
			AsyncPrinter::Printf("Starting straight run\n");
			if (numFrisbees % 2 == 1)//odd, wtf
			{
				driveBackDistance += 11.0/2;
				driveBackDistance += 11.0 * (numFrisbees - 1) / 2.0;
			}
			else
			{
				driveBackDistance += 11.0 * (numFrisbees) / 2.0;
			}
			
			m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, driveBackDistance, 0.9);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, numFrisbees * 11.0 + 20));
			m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.30);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
			//we want to slow down at driveBackDistance - numFrisbees * 11.0 - 20
			AsyncPrinter::Printf("driving aligning again\n");
			
			m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -driveBackDistance + 40 + 40.0 + 34 - 15 + 1.0 - 6, 0.9);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 45));
			m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.5);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
			
				
			AsyncPrinter::Printf("turning\n");
			
	#if USE_COLLECTOR
			m_componentData->collectorData->SlideUp();
			m_componentData->collectorData->StopRollers();
	#endif
			
			m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -91.5, 0.8);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
			SafeWait(0.2, 10);
			AsyncPrinter::Printf("going back to starting pos\n");
			m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -5 * 12.0 - 54 + 11, 0.9); // distance to drive from the midline to the pyramid minus half the robot side with wheels length (33 inches)
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 50)); // threshold before we slow down
			m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.5); // slow down the speed to get a more accurate stop point
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
			//so the distance to the center is 12.0 * 8.0 - 2.0 = 94, 77 to center
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 8.0 - 2.0, 0.90);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.5));
	//		
			AsyncPrinter::Printf("Firing frisbees...\n");
			
			double timeLeft = (Timer::GetFPGATimestamp() - m_autonomousStartTime);
			
			if(timeLeft < 0) timeLeft = 0.0;
			
			FireAllFrisbees(timeLeft);
			
			m_componentData->shooterData->SetShooterSetting(OFF);
			
			AsyncPrinter::Printf("done with\n");
	//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 8, 0.55);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
	//		StopDrive();
	//
	//		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, 143.5, 0.4);
	//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 6.5));
	//		StopDrive();
	//		
	//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 10, 0.3);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 12 * 7));
	//		m_componentData->drivetrainData->setMaxPositionControlSpeed(FORWARD, 0.121);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.255));
	//		StopDrive();
	//		
	//		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -45.5, 0.4);
	//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 8));
	//		StopDrive();
	////
	//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -58, 0.4);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
	//		StopDrive();
	//
	//		m_componentData->collectorData->SlideUp();
	//		m_componentData->collectorData->StopRollers();
	//				
	//		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -38, 0.4);
	//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 8));
	//		StopDrive();
	//		m_componentData->shooterData->SetLauncherAngleLow();
	//
	//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -54 - 12 * 2.5, 0.4);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
	//		AsyncPrinter::Printf("Just moved the rollers up.\n");

	#if FULL_AUTON
			m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
			m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -55, 0.2);
			m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
			StopDrive();
			
			m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
			m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -72, 0.2);
			m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
			StopDrive();
			
			m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
			m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -(90-55), 0.2);
			m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
			StopDrive();

			m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
			m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -38, 0.2);
			m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
			m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.25));
			StopDrive();
	//		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
	//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
			
			//-108.5
			//-2405.8166
			
			//2297.3
			//229.73 per rev
			
			//New code
			//Positive = clockwise
				
	//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 4.0, 0.2);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
	//		StopDrive();
	//		
	//		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, -135, 0.2);
	//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
	//		StopDrive();
	//		
	//		m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
	//		m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 12.0 * 6.0, 0.2);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
	//		m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(TURN, 5));
	//		StopDrive();
	//		
			
	#endif
			
			
			break;
	// recorded routine
	case 5:
		fstream fin("/RecordedRoutine.txt");
		
		if (!fin.is_open())
		{
			AsyncPrinter::Printf("[ERROR] Could not open recorded autonomous routine file\n");
			break;
		}
		queue<Cycle> routine;
		while (!fin.eof())
		{
			Cycle current;
			double forward;
			fin >> forward;
			current.forward = forward;
			double turn;
			fin >> current.turn;
			current.turn = turn;
			fin >> current.collecting;
			fin >> current.shooting;
			fin >> current.angleHigh;
			AsyncPrinter::Printf("next step file %lf\n", current.forward);
			routine.push(current);
		}
		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
		while (!routine.empty())
		{
			AsyncPrinter::Printf("next step\n");
			semTake(loopSem, WAIT_FOREVER);
			Cycle current = routine.front();
			m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, current.forward);
			m_componentData->drivetrainData->setVelocitySetpoint(TURN, current.turn);
			if (current.collecting)
			{
				m_componentData->collectorData->RunRollers();
				m_componentData->collectorData->SlideDown();
			}
			else
			{
				m_componentData->collectorData->StopRollers();
				m_componentData->collectorData->SlideUp();
			}
			if (current.shooting)
			{
				m_componentData->shooterData->SetShooterSetting(CONTINUOUS);
			}
			else
			{
				m_componentData->shooterData->SetShooterSetting(OFF);
			}
			if (current.angleHigh)
			{
				m_componentData->shooterData->SetLauncherAngleHigh();
			}
			else
			{
				m_componentData->shooterData->SetLauncherAngleLow();
			}
		}
		fin.close();
		break;
	}
		
	AsyncPrinter::Printf("Auton done.\n");
	m_isRunning = false;
}

void AutonomousRoutines::StopDrive()
{
		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.0);
		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0.0);
		
	
}

void AutonomousRoutines::ServiceAutoAimBackBoard()
{
//	if(m_componentData->autoAimData->getDesiredX() - m_componentData->autoAimData->getCurrentX() < -m_componentData->autoAimData->getErrorThreshold())
//	{
//		// we're to the right
//		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, (m_componentData->autoAimData->getDesiredX() - m_componentData->autoAimData->getCurrentX() - m_componentData->autoAimData->getErrorThreshold()) / 100.0);
//	}
//	else if(m_componentData->autoAimData->getDesiredX() - m_componentData->autoAimData->getCurrentX() > m_componentData->autoAimData->getErrorThreshold())
//	{
//		// we're to the left
//		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, (m_componentData->autoAimData->getDesiredX() - m_componentData->autoAimData->getCurrentX() + m_componentData->autoAimData->getErrorThreshold()) / 100.0);
//	}
//	else
//	{
//		// we're lined up!
//		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0);
//	}
	if(m_componentData->autoAimData->getDesiredY() - m_componentData->autoAimData->getCurrentY() < -m_componentData->autoAimData->getErrorThreshold())
	{
		// we're too far
		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, (m_componentData->autoAimData->getDesiredY() - m_componentData->autoAimData->getCurrentY() - m_componentData->autoAimData->getErrorThreshold()) / 100.0);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, -0.2);
	}
	else if(m_componentData->autoAimData->getDesiredY() - m_componentData->autoAimData->getCurrentY() > m_componentData->autoAimData->getErrorThreshold())
	{
		// we're too close
		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, (m_componentData->autoAimData->getDesiredY() - m_componentData->autoAimData->getCurrentY() + m_componentData->autoAimData->getErrorThreshold()) / 100.0);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.2);
	}
	else
	{
		// we're lined up!
		m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0);
	}
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
	
	static int e = 0;
	static int lastNum = m_componentData->shooterData->GetNumFrisbeesInStorage();
	static int releaseCount = 0;
	
	AsyncPrinter::Printf("numFrisees %d\n", m_componentData->shooterData->GetNumFrisbeesInStorage() );
	while (m_isRunning && GetFPGATime() < startTime + timeoutSeconds * 1E6 && m_componentData->shooterData->GetNumFrisbeesInStorage() > 0)
	{
		if (lastNum != m_componentData->shooterData->GetNumFrisbeesInStorage())
			e = 0;
		else 
			e++;
		lastNum = m_componentData->shooterData->GetNumFrisbeesInStorage();
		
		if (e > 200 * 2)
		{
			AsyncPrinter::Printf("Timing out");
			releaseCount = 25 * 2;
			e = 0;
		}
		
		if (releaseCount > 0)
		{
			AsyncPrinter::Printf("releasing\n");
			releaseCount--;
			m_componentData->shooterData->SetShooterSetting(OFF);
		}
		else
		{
//			AsyncPrinter::Printf("Firing frisbees\n");
			m_componentData->shooterData->SetShooterSetting(CONTINUOUS);
			
		}
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
