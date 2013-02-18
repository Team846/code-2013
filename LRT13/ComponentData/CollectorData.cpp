#include "CollectorData.h"

using namespace data;
using namespace data::collector;

CollectorData::CollectorData()
{
	m_collectorState = CollectorState::UP;
}

void CollectorData::RunRollers()
{
	m_running = true;
}

void CollectorData::StopRollers()
{
	m_running = false;
}

bool CollectorData::ShouldRunRollers()
{
	return m_running;
}

void CollectorData::Slide(CollectorState::Enum position)
{
	m_collectorState = position;
}

void CollectorData::SlideDown()
{
	Slide(CollectorState::DOWN);
}

void CollectorData::SlideUp()
{
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