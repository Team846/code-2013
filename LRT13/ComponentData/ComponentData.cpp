#include "ComponentData.h"
#include "DrivetrainData.h"

using namespace data;
using namespace drivetrain;

ComponentData* ComponentData::GetInstance()
{
	if (!m_instance)
		m_instance = new ComponentData();
	return m_instance;
}

ComponentData::ComponentData()
: drivetrainData(new DrivetrainData())
{
	
}
