#include "CollectorRollersData.h"

CollectorRollersData::CollectorRollersData() :
	ComponentData("CollectorRollersData")
{
	ResetCommands();
}

CollectorRollersData* CollectorRollersData::Get()
{
	return (CollectorRollersData*) ComponentData::GetComponentData("CollectorRollersData");
}

void CollectorRollersData::ResetCommands()
{
	m_running = false;
	m_direction = FORWARD;
}

void CollectorRollersData::Log()
{
	LogToFile(&m_running, "Running");
	LogToFile(&m_direction, "Direction");
}

void CollectorRollersData::SetDirection(CollectorRollersData::Direction direction)
{
	m_direction = direction;
}

void CollectorRollersData::SetRunning(bool run)
{
	m_running = run;
}

CollectorRollersData::Direction CollectorRollersData::GetDirection()
{
	return m_direction;
}

bool CollectorRollersData::IsRunning()
{
	return m_running;
}
