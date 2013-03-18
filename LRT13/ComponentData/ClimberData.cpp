#include "ClimberData.h"
using namespace data;
using namespace climber;

ClimberData::ClimberData()
{
	m_desiredClimbingStep = INTENDED_IDLE;
	m_shouldContinueClimbing = false;
	m_shouldForceContinueClimbing = false;
	
	m_shouldChangeArmState = false;
	m_shouldChangeAngleState = false;
	
	m_shouldWinchPawlGoDown = false;
	m_shouldWinchPawlGoUp = false;
	
	m_shouldPTOEngage = false;
	m_shouldPTODisEngage = false;

}

humanState ClimberData::getDesiredClimbingStep()
{
	return m_desiredClimbingStep;
}
void ClimberData::setDesiredClimbingStep(humanState newState)
{
	m_desiredClimbingStep = newState;
}
bool ClimberData::shouldContinueClimbing()
{
	return m_shouldContinueClimbing;
}

bool ClimberData::shouldForceContinueClimbing()
{
	return m_shouldForceContinueClimbing;
}

void ClimberData::setShouldContinueClimbing(bool shouldContinue)
{
	m_shouldContinueClimbing = shouldContinue;
}

void ClimberData::setShouldForceContinueClimbing(bool shouldContinue)
{
	m_shouldContinueClimbing = shouldContinue;
}


bool ClimberData::shouldChangeArmState()
{
	if (m_shouldChangeArmState)
	{
		m_shouldChangeArmState = false;
		return true;
	}
	return false;
}

bool ClimberData::changeArmState()
{
	m_shouldChangeArmState = true;
}

bool ClimberData::shouldChangeAngleState()
{
	
	if (m_shouldChangeAngleState)
	{
		m_shouldChangeAngleState = false;
		return true;
	}
	return false;
}

void ClimberData::changeAngleState()
{
	m_shouldChangeAngleState = true;
}

bool ClimberData::shouldWinchPawlGoDown()
{
	if (m_shouldWinchPawlGoDown)
	{
		m_shouldWinchPawlGoDown = false;
		return true;
	}
	return false;
}

bool ClimberData::shouldWinchPawlGoUp()
{
	if (m_shouldWinchPawlGoUp)
	{
		m_shouldWinchPawlGoUp = false;
		return true;
	}
	return false;
	
}

void ClimberData::winchPawlDown()
{
	
	m_shouldWinchPawlGoDown = true;
}

void ClimberData::winchPawlUp()
{
	m_shouldWinchPawlGoUp = true;
}

bool ClimberData::shouldPTOChangeDisengage()
{
	if (m_shouldPTOEngage)
	{
		m_shouldPTOEngage = false;
		return true;
	}
	return false;
}

bool ClimberData::shouldPTOChangeEngage()
{
	if (m_shouldPTODisEngage)
	{
		m_shouldPTODisEngage = false;
		return true;
	}
	return false;
}

bool ClimberData::EngagePTO()
{
	m_shouldPTOEngage = true;
}

bool ClimberData::DisengagePTO()
{
	m_shouldPTODisEngage = true;
}
		
bool ClimberData::shouldDebug()
{
	return m_shoulddebug;
}

void ClimberData::enableDebug()
{
	m_shoulddebug = true;
}
		
void ClimberData::disableDebug()
{
	m_shoulddebug = false;
}
		
