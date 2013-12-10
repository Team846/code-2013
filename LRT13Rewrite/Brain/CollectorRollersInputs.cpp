#include "CollectorRollersInputs.h"

CollectorRollersInputs::CollectorRollersInputs()
{
	m_driver_stick = LRTDriverStation::Instance()->GetDriverStick();
	m_operator_stick = LRTDriverStation::Instance()->GetOperatorStick();
	
	collectorRollersData = CollectorRollersData::Get();
}

void CollectorRollersInputs::Update()
{
	if (m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::COLLECTOR_SLIDE)
			|| m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::COLLECT_OPERATOR))
	{
		collectorRollersData->SetDirection(CollectorRollersData::FORWARD);
		collectorRollersData->SetRunning(true);
	}
	else if (m_operator_stick->IsButtonDown(DriverStationConfig::JoystickButtons::PURGE))
	{
		collectorRollersData->SetDirection(CollectorRollersData::REVERSE);
		collectorRollersData->SetRunning(true);
	}
	else
	{
		collectorRollersData->SetRunning(false);
	}
}
