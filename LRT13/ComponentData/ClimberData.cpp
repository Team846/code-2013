#include "ClimberData.h"
using namespace data;
using namespace climber;

ClimberData::ClimberData()
{
	m_desiredClimbingStep = INTENDED_IDLE;
	m_shouldContinueClimbing = false;
	m_shouldForceContinueClimbing = false;

	m_shouldWinchPawlGoUp = false;
	m_shouldWinchPawlGoDown = false;
	
	m_shouldPTOEngage = false;
	m_shouldPTODisEngage = false;
	m_PTOEngaged = false;

	m_shouldPotentiallyAbort = false;
	
	m_armState = false;
	
	m_shouldHooksChange = false;

	m_winchPawlCurrent = 0.0;
	m_shouldChangeArm = false;
	m_desiredState = NOTHING;
	m_currentState = NOTHING;
	m_waitingState = NOTHING;
}


bool ClimberData::shouldPotentiallyAbort()
{
	return m_shouldPotentiallyAbort;
}

void ClimberData::setShouldPotentiallyAbort(bool shouldAbort)
{
	m_shouldPotentiallyAbort = shouldAbort;
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

void ClimberData::changeArmState()
{
	m_armState = !m_armState;
	m_shouldChangeArm = true;
}

void ClimberData::extendArm()
{
	m_armState = true;
	m_shouldChangeArm = true;
}

void ClimberData::retractArm()
{
	m_armState = false;
	m_shouldChangeArm = true;
}

bool ClimberData::shouldArmsUp()
{
	return m_armState;
}

bool ClimberData::shouldArmsChange()
{
	if (m_shouldChangeArm)
	{
		m_shouldChangeArm = false;
		return true;
	}
	return false;
}

bool ClimberData::shouldWinchPawlGoUp()
{
	return m_shouldWinchPawlGoUp;
}

bool ClimberData::shouldWinchPawlGoDown()
{
	return m_shouldWinchPawlGoDown;
}

void ClimberData::winchPawlUp()
{
	m_shouldWinchPawlGoUp = true;
}

void ClimberData::winchPawlDown()
{
	m_shouldWinchPawlGoDown = true;
}

void ClimberData::winchPawlInactive()
{
	m_shouldWinchPawlGoUp = m_shouldWinchPawlGoDown = false;
}

double ClimberData::winchPawlCurrent()
{
	return m_winchPawlCurrent;
}

void ClimberData::setWinchPawlCurrent(double value)
{
	m_winchPawlCurrent = value;
}

bool ClimberData::shouldPTOChangeDisengage()
{
	if (m_shouldPTODisEngage)
	{
		m_shouldPTODisEngage = false;
		return true;
	}
	return false;
}

bool ClimberData::shouldPTOChangeEngage()
{
	if (m_shouldPTOEngage)
	{
		m_shouldPTOEngage = false;
		return true;
	}
	return false;
}

void ClimberData::EngagePTO()
{
	m_shouldPTOEngage = true;
}

void ClimberData::DisengagePTO()
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

void ClimberData::setDesiredState(state target)
{
	m_desiredState = target;
}

state ClimberData::getDesiredState()
{
	return m_desiredState;
}

void ClimberData::setCurrentState(state newState)
{
	m_currentState = newState;
}

state ClimberData::getCurrentState()
{
	return m_currentState;
}

void ClimberData::setWaitingState(state newState)
{
	m_waitingState = newState;
}

state ClimberData::getWaitingState()
{
	return m_waitingState;
}

void ClimberData::setShouldHooksChange(bool hooksDown)
{
	m_shouldHooksChange = hooksDown;
}

bool ClimberData::shouldHooksChange()
{
	if (m_shouldHooksChange)
	{
		m_shouldHooksChange = false;
		return true;
	}
	return false;
}

bool ClimberData::isPTOEngaged()
{
	return m_PTOEngaged;
}

void ClimberData::setPTOEngaged(bool engaged)
{
	m_PTOEngaged = engaged;
}
