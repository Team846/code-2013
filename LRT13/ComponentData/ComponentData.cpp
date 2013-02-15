#include "ComponentData.h"
#include "DrivetrainData.h"
#include "ShooterData.h"
#include "ConfigLoaderData.h"
#include "ShooterData.h"
#include "CollectorData.h"
#include "PTOData.h"

using namespace data;
using namespace drivetrain;
using namespace shooter;
using namespace collector;
using namespace pto;
using namespace configloader;

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
: drivetrainData(new DrivetrainData()),
  shooterData(new ShooterData()),
  collectorData(new CollectorData()),
  ptoData(new PTOData()),
  configLoaderData(new ConfigLoaderData())
{
	
}
