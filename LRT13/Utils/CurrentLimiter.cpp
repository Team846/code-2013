#include "CurrentLimiter.h"

float CurrentLimiter::GetMaximumDutyCycleDelta(float currentCurrent, float desired, float current_cap)
{
	return current_cap - currentCurrent;
}
