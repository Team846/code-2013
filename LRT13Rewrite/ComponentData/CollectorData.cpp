#include "CollectorData.h"

using namespace data;
using namespace data::collector;

CollectorData::CollectorData()
{
	m_collectorState = CollectorState::UP;
	m_running = false;
}

void CollectorData::RunRollers()
{
	m_running = true;
	dir = 1;
}

void CollectorData::RunRollersBackwards()
{
	m_running = true;
	dir = -1;
}

void CollectorData::StopRollers()
{
	m_running = false;
}

bool CollectorData::ShouldRunRollers()
{
	return m_running;
}

int CollectorData::RollerDirection()
{
	return dir;
}

void CollectorData::Slide(CollectorState::Enum position)
{
	m_collectorState = position;
}

void CollectorData::SlideDown()
{
	shouldStupidMoveDown = false;
	Slide(CollectorState::DOWN);
}

void CollectorData::SlideUp()
{
	shouldStupidMoveDown = false;
	Slide(CollectorState::UP);
}

CollectorState::Enum CollectorData::GetCollectorPosition()
{
	return m_collectorState;
}

bool CollectorData::IsDown()
{
	return GetCollectorPosition() == CollectorState::DOWN;
}

bool CollectorData::IsUp()
{
	return GetCollectorPosition() == CollectorState::UP;
}

void CollectorData::SetExtendTime(int ticks)
{
	m_extendTicks = ticks;
}

void CollectorData::SetRetractTime(int ticks)
{
	m_retractTicks = ticks;
}

int CollectorData::GetExtendTime()
{
	return m_extendTicks;
}

int CollectorData::GetRetractTime()
{
	return m_retractTicks;
}
