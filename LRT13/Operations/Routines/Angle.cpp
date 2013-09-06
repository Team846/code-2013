#include "Angle.h"
#include "../../ComponentData/ShooterData.h"

Angle::Angle(bool high)
{
	m_high = high;
}

void Angle::Run()
{
	if (m_high)
		m_componentData->shooterData->SetLauncherAngleHigh();
	else
		m_componentData->shooterData->SetLauncherAngleLow();
}

bool Angle::Completed()
{
	return true;
}
