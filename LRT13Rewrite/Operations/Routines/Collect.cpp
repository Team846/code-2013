#include "Collect.h"
#include "../../ComponentData/CollectorData.h"

Collect::Collect(bool collect)
{
	m_collect = collect;
}

void Collect::Run()
{
	if (m_collect)
	{
		m_componentData->collectorData->SlideDown();
		m_componentData->collectorData->RunRollers();
	}
	else
	{
		m_componentData->collectorData->SlideUp();
		m_componentData->collectorData->StopRollers();
	}
}

bool Collect::Completed()
{
	return true;
}
