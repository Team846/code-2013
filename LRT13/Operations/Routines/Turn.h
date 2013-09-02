#ifndef Turn_H_
#define Turn_H_

#include "Routine.h"

class Turn : public Routine
{
public:
	Turn(double angle, double maxSpeed = 1.0, double errorThreshold = 0.5);
	
	virtual void Run();
	
	virtual bool Completed();
private:
	double m_angle;
	double m_maxSpeed;
	double m_errorThreshold;
};

#endif
