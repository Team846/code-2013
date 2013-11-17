#include "RoutineRecorderData.h"

using namespace data;
using namespace data::routinerecorder;

RoutineRecorderData::RoutineRecorderData()
{
	m_recordRequested = false;
}

bool RoutineRecorderData::ShouldRecord()
{
	return m_recordRequested;
}

void RoutineRecorderData::ShouldRecord(bool record)
{
	m_recordRequested = record;
}
