#ifndef DRIVE_H_
#define DRIVE_H_

#include "Routine.h"

class Drive : public Routine
{
public:
	Drive(double distance, double maxSpeed = 1.0, double errorThreshold = 0.5);
	
	virtual void Run();
	
	virtual bool Completed();
private:
	double m_distance;
	double m_maxSpeed;
	double m_errorThreshold;
};

#endif
