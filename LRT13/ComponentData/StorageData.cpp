#include "StorageData.h"

using namespace data::storage;

StorageData::StorageData()
{
	m_frisbeeCount = 0;
}

StorageData::~StorageData()
{
	
}

void StorageData::IncrementFrisbeeCount()
{
	m_frisbeeCount++;
}

void StorageData::DecrementFrisbeeCount()
{
	m_frisbeeCount--;
}

int StorageData::GetFrisbeeCount()
{
	return m_frisbeeCount;
}
