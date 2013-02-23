#include "ShooterData.h"
#include "ComponentData.h"

using namespace data;
using namespace data::shooter;

ShooterData::ShooterData()
{
	// these arrays are statically allocated and thus cannot be deallocated... -tp
	memset(atSpeed, false, sizeof(atSpeed));
	memset(speed, 0, sizeof(speed));
	
	frisbeeCounter = 0;
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

void ShooterData::SetAcceptableSpeedError(double error, int roller)
{
	acceptableSpeedError[roller] = error;
}

double ShooterData::GetAcceptableSpeedError(int roller)
{
	return acceptableSpeedError[roller];
}

void ShooterData::SetLauncherAngleHigh()
{
	extendLauncher = true;
}

void ShooterData::SetLauncherAngleLow()
{
	extendLauncher = false;
}

bool ShooterData::ShouldLauncherBeHigh()
{
	return extendLauncher;
}

int ShooterData::GetNumFrisbeesInStorage()
{
	return frisbeeCounter;
}

void ShooterData::SetNumFrisbeesInStorage(int numFrisbees)
{
	frisbeeCounter = numFrisbees;
}

void ShooterData::IncrementFrisbeeCounter()
{
	frisbeeCounter++;
}

void ShooterData::DecrementFrisbeeCounter()
{
	frisbeeCounter--;
}
