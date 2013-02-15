#include "PTOData.h"

using namespace data;
using namespace data::pto;

PTOData::PTOData()
{
	m_engaged = false;
}

void PTOData::Engage()
{
	m_engaged = true;
}

void PTOData::Disengage()
{
	m_engaged = false;
}

bool PTOData::IsEngaged()
{
	return m_engaged;
}
