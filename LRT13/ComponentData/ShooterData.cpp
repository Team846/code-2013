#include "ShooterData.h"
#include "ComponentData.h"

using namespace data;
using namespace data::shooter;

ShooterData::ShooterData()
{
	memset(atSpeed, false, sizeof(atSpeed));
	memset(speed, 0, sizeof(speed));
}

void ShooterData::setDesiredSpeed(double desiredSpeed, Roller roller)
{
	speed[roller] = desiredSpeed;
}

double ShooterData::getDesiredSpeed(Roller roller)
{
	return speed[roller];
}

void ShooterData::setAtSpeed(bool value, Roller roller)
{
	atSpeed[roller] = value;
}

bool ShooterData::isAtSpeed(Roller roller)
{
	return atSpeed[roller];
}
