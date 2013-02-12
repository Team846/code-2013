#include "ComponentData.h"
#include "DrivetrainData.h"
#include "ShooterData.h"
#include "ConfigLoaderData.h"

using namespace data;
using namespace drivetrain;
using namespace shooter;
using namespace configLoader;

ComponentData* ComponentData::m_instance = NULL;

ComponentData* ComponentData::GetInstance()
{
	if (!m_instance)
		m_instance = new ComponentData();
	
	return m_instance;
}

void ComponentData::Finalize()
{
	DELETE(m_instance);
}

ComponentData::ComponentData()
: drivetrainData(new DrivetrainData()), shooterData(new ShooterData()), configLoaderData(new ConfigLoaderData())
{
	
}
