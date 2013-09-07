#include "Shoot.h"
#include "../../ComponentData/ShooterData.h"

Shoot::Shoot(int count, double timeout)
{
	m_count = count;
	m_timeout = timeout;
}

void Shoot::Run()
{
	m_timer.Start();
	m_startingCount = m_componentData->shooterData->GetFrisbeeCounter();
	m_componentData->shooterData->SetShooterSetting(data::shooter::CONTINUOUS);
}

bool Shoot::Completed()
{
	return (m_timeout && m_timer.Get() > m_timeout) || m_componentData->shooterData->GetFrisbeeCounter() >= m_startingCount + m_count;
}

void Shoot::Stop()
{
	m_componentData->shooterData->SetShooterSetting(data::shooter::OFF);
}
