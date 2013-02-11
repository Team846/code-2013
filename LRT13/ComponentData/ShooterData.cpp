#include "ShooterData.h"
#include "ComponentData.h"

using namespace data;
using namespace data::shooter;

ShooterData::ShooterData()
{
	
	state = data::shooter::RUNNING;
	atSpeed = false;
	speed = 1500;
	desiredTarget = data::shooter::SLOW;
	
}
