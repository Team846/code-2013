#include "ClimberData.h"
using namespace data;
using namespace climber;

ClimberData::ClimberData()
{
	m_desiredClimbingStep = INTENDED_IDLE;
	m_shouldContinueClimbing = false;
	m_shouldForceContinueClimbing = false;
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
