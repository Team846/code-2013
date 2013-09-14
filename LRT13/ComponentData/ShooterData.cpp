#include "ShooterData.h"
#include "../Utils/AsyncPrinter.h"
#include "ComponentData.h"

using namespace data;
using namespace data::shooter;

ShooterData::ShooterData()
{
	// these arrays are statically allocated and thus cannot be deallocated... -tp
	memset(atSpeed, false, sizeof(atSpeed));
	memset(speed, 0, sizeof(speed));
	
	frisbeeCounter = 0;
	
	shooter_setting = OFF;
	extendLauncher = true;
	
	speedOffset = 1.0;
	
	m_enabled = true;
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

int ShooterData::GetFrisbeeCounter()
{
	return frisbeeCounter;
}

void ShooterData::SetFrisbeeCounter(int numFrisbees)
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

void ShooterData::SetShooterSetting(ShooterSetting s)
{
	shooter_setting = s;
}

int ShooterData::GetShooterSetting()
{
	return shooter_setting;
}

void ShooterData::SetEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool ShooterData::IsEnabled()
{
	return m_enabled;
}

void ShooterData::increaseSpeed()
{
	m_shouldIncreaseSpeed = true;
}

void ShooterData::decreaseSpeed()
{
	m_shouldDecreaseSpeed = true;
}

bool ShooterData::shouldIncreaseSpeedOnce()
{
	if (m_shouldIncreaseSpeed)
	{
		m_shouldIncreaseSpeed = false;
		return true;
	}
	else 
		return false;
}

bool ShooterData::shouldDecreaseSpeedOnce()
{
	if (m_shouldDecreaseSpeed)
	{
		m_shouldDecreaseSpeed = false;
		return true;
	}
	else 
		return false;
	
}

void ShooterData::SetSpeedOffset(double offset)
{
	speedOffset = offset;
}

double ShooterData::GetSpeedOffset()
{
	return speedOffset;
}
