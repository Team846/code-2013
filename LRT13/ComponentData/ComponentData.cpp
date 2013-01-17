#include "ComponentData.h"
#include "DrivetrainData.h"
#include "ConfigLoaderData.h"

using namespace data;
using namespace drivetrain;
using namespace configLoader;

ComponentData* ComponentData::m_instance = NULL;

ComponentData* ComponentData::GetInstance()
{
	if (!m_instance)
		m_instance = new ComponentData();
	return m_instance;
}

ComponentData::ComponentData()
: drivetrainData(new DrivetrainData()), configLoaderData(new ConfigLoaderData())
{
	
}
