#include "RoutineRecorder.h"

using namespace data::routinerecorder;
using namespace data::drivetrain;
using namespace data::collector;
using namespace data::shooter;

RoutineRecorder::RoutineRecorder()
: Component("Drivetrain", -1, true),
  m_driveEncoders(DriveEncoders::GetInstance()),
  m_file("/RecordedRoutine.txt")
{
	m_lastRecording = false;
}

RoutineRecorder::~RoutineRecorder()
{
	
}

void RoutineRecorder::onEnable()
{
	
}

void RoutineRecorder::onDisable()
{
	if (m_file.is_open())
		m_file.close();
	while(!m_routine.empty())
		m_routine.pop();
}

void RoutineRecorder::enabledPeriodic()
{
	if (m_componentData->routineRecorderData->ShouldRecord())
	{
		m_lastRecording = true;
		AsyncPrinter::Printf("Recording Routine...\n");
		Cycle current;
		current.forward = 0;
		current.turn = 0;
		current.collecting = false;
		current.shooting = false;
		current.angleHigh = false;
		if (m_componentData->drivetrainData->getControlMode(FORWARD) == VELOCITY_CONTROL)
			current.forward = m_componentData->drivetrainData->getVelocitySetpoint(FORWARD);
		else if (m_componentData->drivetrainData->getControlMode(FORWARD) == OPEN_LOOP)
			current.forward = m_componentData->drivetrainData->getOpenLoopOutput(FORWARD);
		if (m_componentData->drivetrainData->getControlMode(TURN) == VELOCITY_CONTROL)
			current.turn = m_componentData->drivetrainData->getVelocitySetpoint(TURN);
		else if (m_componentData->drivetrainData->getControlMode(TURN) == OPEN_LOOP)
			current.turn = m_componentData->drivetrainData->getOpenLoopOutput(TURN);
		current.collecting = m_componentData->collectorData->ShouldRunRollers() && m_componentData->collectorData->IsDown();
		current.shooting = m_componentData->shooterData->GetShooterSetting() == CONTINUOUS;
		current.angleHigh = m_componentData->shooterData->ShouldLauncherBeHigh();
		m_routine.push(current);
	}
	else
	{
		if (m_lastRecording)
		{
			AsyncPrinter::Printf("Saving Routine...\n");
			m_file.open("/RecordedRoutine.txt");
			while (!m_routine.empty())
			{
				Cycle current = m_routine.front();
				m_file << current.forward << ' ' << current.turn << ' ' << current.collecting << ' ' << current.shooting << ' ' << current.angleHigh << "\n";
				m_routine.pop();
			}
			m_file.close();
		}
		if (m_file.is_open())
			m_file.close();
		while(!m_routine.empty())
			m_routine.pop();
		m_lastRecording = false;
	}
}

void RoutineRecorder::disabledPeriodic()
{
	if (m_file.is_open())
		m_file.close();
	while(!m_routine.empty())
		m_routine.pop();
}
