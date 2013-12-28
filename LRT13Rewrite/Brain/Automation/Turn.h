#ifndef TURN_H_
#define TURN_H_

#include "Automation.h"
#include "../../ComponentData/DrivetrainData.h"

class Turn : public Automation
{
public:
	Turn(double angle, double maxSpeed = 1.0, double errorThreshold = 0.5);

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();
	
private:
	DrivetrainData *m_drivetrain;
	
	double m_angle;
	double m_maxSpeed;
	double m_errorThreshold;
};

#endif
