#include "Collect.h"
#include "../../ComponentData/CollectorData.h"

Collect::Collect(bool collect)
{
	m_collect = collect;
}

void Collect::Run()
{
	m_componentData->collectorData->SlideDown();
	m_componentData->collectorData->RunRollers();
}

bool Collect::Completed()
{
	return true;
}
