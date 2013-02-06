#include "AutonomousRoutines.h"
#include "../ComponentData/ComponentData.h"
#include "AutoActions.h"
#include "../Config/DriverStationConfig.h"

using namespace data;
using namespace data::drivetrain;

const AutonomousRoutines::autonStage AutonomousRoutines::DRIVE_FORWARD_AND_SHOOT[] = {INIT, DRIVE_FORWARD, AUTO_AIM, SHOOT, COMPLETED};

AutonomousRoutines::AutonomousRoutines(char * taskName, INT32 priority) :
	SynchronizedProcess(taskName, priority)
{
	m_componentData = ComponentData::GetInstance();
	m_currentStage = INIT;
	
	m_autonActionCount = 1;
	m_autonActions[0] = new PauseAction(100);
	
	m_autoActions = new AutoActions();
	
	m_isRunning = false;
}

AutonomousRoutines::~AutonomousRoutines()
{

}

INT32 AutonomousRoutines::Tick()
{
//	for(int i = 0; i < m_autonActionCount; i++)
//	{
//		m_autonActions[i]->Run();
//	}
	// Use position control in autonomous mode
	m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
	m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
	AsyncPrinter::Printf("isRunning: %d", m_isRunning);
	if (!m_isRunning) // Does not run again (even after routine is completed) until game state is changed and changed back (Stop() is called).
	{
		LoadQueue();
		Autonomous(); // Operation does not complete until entire routine is completed or when Stop() is called.
	}
	// Tick() does not finish until entire routine is completed or Stop() is called.
	return 0;
}

void AutonomousRoutines::Autonomous()
{	
	m_isRunning = true;
	while (!m_routine.empty() && m_isRunning)
	{
		m_currentStage = m_routine.front();
		switch(m_currentStage)
		{
		case INIT:
			break;
		case DRIVE_FORWARD:
			m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 25.0, 0.1);
			m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, 0.0, 0.0);
			// Take semaphore to wait until operation completes
			semTake(m_componentData->drivetrainData->positionOperationSemaphore(FORWARD, 0), WAIT_FOREVER);
			semGive(m_componentData->drivetrainData->positionOperationSemaphore(FORWARD, 0));
			
			AsyncPrinter::DbgPrint("Drive forward operation completed.");
			break;
		case AUTO_AIM:
			while(!m_autoActions->AutoAim());
			
			AsyncPrinter::DbgPrint("Autoaim done");
			break;
		case SHOOT:
			break;
		case COMPLETED:
			m_autoActions->Reset();
			break;
		}
		m_routine.pop();
		
		Wait(0.1);
	}
	// Routine complete. Keep m_isRunning as true to prevent routine from running again when Tick() is called while still in autonomous mode.
}

void AutonomousRoutines::LoadQueue()
{
	while (!m_routine.empty())
		m_routine.pop();

	const autonStage* stages;
	uint8_t selected = (uint8_t) DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_SELECT);
	switch(selected)
	{
	case 1:
		stages = DRIVE_FORWARD_AND_SHOOT;
		break;
	}
	for (int i = 0;; i++)
	{
		m_routine.push(stages[i]);
		if (stages[i] == COMPLETED)
			break;
	}
}

void AutonomousRoutines::Stop()
{
	m_isRunning = false; // Allows routine to start again when Tick() is called (autonomous mode is re-entered).
}

bool AutonomousRoutines::IsRunning()
{
	return m_isRunning;
}
