#include "ClimberData.h"
using namespace data;
using namespace climber;

ClimberData::ClimberData()
{
	m_desiredClimbingStep = INTENDED_IDLE;
	m_shouldContinueClimbing = false;
	m_shouldForceContinueClimbing = false;

	m_shouldChangeAngleState = false;
	
	m_shouldWinchPawlGoUp = false;
	m_shouldWinchPawlGoDown = false;
	
	m_shouldPTOEngage = false;
	m_shouldPTODisEngage = false;

	m_shouldPotentiallyAbort = false;
	
	m_shouldExtendArm = false;
	m_shouldExtendHooks = false;
	
	m_winchPawlCurrent = 0.0;
	
	m_desiredState = NOTHING;
	m_currentState = INACTIVE;
	m_waitingState = NOTHING;
}


bool ClimberData::shouldPotentiallyAbort()
{
	return m_shouldPotentiallyAbort;
}

bool ClimberData::setShouldPotentiallyAbort(bool shouldAbort)
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

void ClimberData::extendHooks()
{
	m_shouldExtendHooks = true;
}

void ClimberData::retractHooks()
{
	m_shouldExtendHooks = false;
}

bool ClimberData::shouldExtendHooks()
{
	return m_shouldExtendHooks;
}

bool ClimberData::changeArmState()
{
	m_shouldExtendArm = !m_shouldExtendArm;
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

void ClimberData::extendArm()
{
	m_shouldExtendArm = true;
}

void ClimberData::retractArm()
{
	m_shouldExtendArm = false;
}

bool ClimberData::shouldExtendArm()
{
	return m_shouldExtendArm;
}

void ClimberData::changeAngleState()
{
	m_shouldChangeAngleState = true;
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
