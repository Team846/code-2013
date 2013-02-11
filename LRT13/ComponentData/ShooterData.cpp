#include "ShooterData.h"
#include "ComponentData.h"

using namespace data;
using namespace data::shooter;

/*
 * @author Varun Parthasarathy
 */

ShooterData::ShooterData()
{
	
	state = data::shooter::RUNNING;
	atSpeed = false;
	speed = 1500;
	desiredTarget = data::shooter::SLOW;
	
}
