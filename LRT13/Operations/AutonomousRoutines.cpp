#include "AutonomousRoutines.h"
#include "../ComponentData/ComponentData.h"
#include "AutoActions.h"
#include "../Config/DriverStationConfig.h"
#include "../Utils/AsyncPrinter.h"

using namespace data;
using namespace data::drivetrain;

AutonomousRoutines::AutonomousRoutines()
{
	m_componentData = ComponentData::GetInstance();
	
	m_autoActions = new AutoActions();
	
	m_isRunning = false;
}

AutonomousRoutines::~AutonomousRoutines()
{

}

void AutonomousRoutines::Tick()
{
	switch (m_autoActions->GetCurrentAutonomousAction())
	{
	case AutoActions::AUTO_AIM_BACKBOARD:
		ServiceAutoAimBackBoard();
		break;
	case AutoActions::AUTO_AIM_PYRAMID:
		ServiceAutoAimPyramid();
		break;
	case AutoActions::FEEDER_STATION_APPROACH:
		ServiceFeederStationApproach();
		break;
	case AutoActions::NO_ADV_AUTOMATION:
		//ne faire rien
		break;
	}
}

void AutonomousRoutines::Autonomous()
{
}


void AutonomousRoutines::Stop()
{
	m_isRunning = false; 
}

void AutonomousRoutines::Start()
{
	m_isRunning = true; 
	//faire qqch d'autre dans l'avenir
}

bool AutonomousRoutines::IsRunning()
{
	return m_isRunning;
}
