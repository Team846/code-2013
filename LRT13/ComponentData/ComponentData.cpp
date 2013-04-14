#include "ComponentData.h"
#include "DrivetrainData.h"
#include "ClimberData.h"
#include "ShooterData.h"
#include "ConfigLoaderData.h"
#include "ShooterData.h"
#include "CollectorData.h"
#include "LEDIndicatorData.h"
#include "AutoAimData.h"
#include "WinchPawlData.h"

using namespace data;
using namespace drivetrain;
using namespace shooter;
using namespace collector;
using namespace climber;
using namespace configloader;
using namespace indicators;
using namespace autoaim;

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
  climberData(new ClimberData()),
  configLoaderData(new ConfigLoaderData()),
  ledIndicatorData(new LEDIndicatorData()),
  autoAimData(new AutoAimData()),
  winchPawlData(new WinchPawlData())
{
	
}

ComponentData::~ComponentData()
{
	delete drivetrainData;
	delete shooterData;
	delete collectorData;
	delete climberData;
	delete configLoaderData;
	delete ledIndicatorData;
	delete autoAimData;
	delete winchPawlData;
}
