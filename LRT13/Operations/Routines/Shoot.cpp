#include "Shoot.h"
#include "../../ComponentData/ShooterData.h"

Shoot::Shoot(int count, double timeout)
{
	m_count = count;
	m_timeout = timeout;
}

void Shoot::Run()
{
	m_componentData->shooterData->SetShooterSetting(data::shooter::CONTINUOUS);
}

bool Shoot::Completed()
{
	// TODO: Fire count
	return true;
}

void Shoot::Stop()
{
	m_componentData->shooterData->SetShooterSetting(data::shooter::OFF);
}
