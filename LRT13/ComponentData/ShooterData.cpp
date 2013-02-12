#include "ShooterData.h"
#include "ComponentData.h"

using namespace data;
using namespace data::shooter;

ShooterData::ShooterData()
{
	// these arrays are statically allocated and thus cannot be deallocated... -tp
	memset(atSpeed, false, sizeof(atSpeed));
	memset(speed, 0, sizeof(speed));
}

void ShooterData::SetDesiredSpeed(double desiredSpeed, Roller roller)
{
	speed[roller] = desiredSpeed;
}

double ShooterData::GetDesiredSpeed(Roller roller)
{
	return speed[roller];
}

void ShooterData::SetAtSpeed(bool value, Roller roller)
{
	atSpeed[roller] = value;
}

bool ShooterData::IsAtSpeed(Roller roller)
{
	return atSpeed[roller];
}
