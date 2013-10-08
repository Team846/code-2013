#ifndef ARC_H_
#define ARC_H_

#include "Routine.h"

class Arc : public Routine
{
public:
	Arc(double distance, double angle, double maxSpeed = 1.0, double errorThreshold = 0.5);
	
	virtual void Run();
	
	virtual bool Completed();
private:
	double m_distance;
	double m_angle;
	double m_maxSpeed;
	double m_errorThreshold;
};

#endif
