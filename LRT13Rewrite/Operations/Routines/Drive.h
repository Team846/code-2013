#ifndef DRIVE_H_
#define DRIVE_H_

#include "Routine.h"

class Drive : public Routine
{
public:
	Drive(double distance, double maxSpeed = 1.0, double errorThreshold = 0.5, bool continuous = false);
	
	virtual void Run();
	
	virtual bool Completed();
	
	virtual void Stop();
private:
	double m_distance;
	double m_maxSpeed;
	double m_errorThreshold;
	bool m_continuous;
};

#endif
