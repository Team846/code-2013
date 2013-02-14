#include "AutonomousRoutines.h"
#include "../ComponentData/ComponentData.h"
#include "AutoActions.h"
#include "../Config/DriverStationConfig.h"
#include "../Utils/AsyncPrinter.h"

using namespace data;
using namespace data::drivetrain;

const AutonomousRoutines::autonStage AutonomousRoutines::DRIVE_FORWARD_AND_SHOOT[5] = {INIT, DRIVE_FORWARD, AUTO_AIM, SHOOT, COMPLETED};

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
			while(true)
			{
				AsyncPrinter::Printf("Starting\n");
				m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, 15, 0.1);
				m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, 0.0, 0.0);
				// Take semaphore to wait until operation completes
				while (!m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.05), 0.2))
					m_componentData->drivetrainData->DebugPrintPosition(FORWARD);
				AsyncPrinter::Printf("Done\n");
//				m_componentData->drivetrainData->DebugPrintPosition(FORWARD);
				
				m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, -15, 0.1);
				m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, 0.0, 0.0);
				// Take semaphore to wait until operation completes
				while (!m_componentData->drivetrainData->cleanWaitForSem(m_componentData->drivetrainData->createPositionOperationSemaphore(FORWARD, 0.05), 0.2))
					m_componentData->drivetrainData->DebugPrintPosition(FORWARD);
			}
			break;
		case AUTO_AIM:
			while(!m_autoActions->AutoAim());
			break;
		case SHOOT:
			break;
		case COMPLETED:
			m_autoActions->Reset();
			break;
		}
		m_routine.pop();
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
	case 0:
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
