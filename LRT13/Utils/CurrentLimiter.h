#ifndef CURRENT_LIMITER_H_
#define CURRENT_LIMITER_H_

class CurrentLimiter
{
public:
	float GetMaximumDutyCycleDelta(float speed, float limit, float voltage);
};

#endif
