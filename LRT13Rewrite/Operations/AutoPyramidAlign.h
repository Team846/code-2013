#ifndef AUTO_PYRAMID_ALIGN_H_
#define AUTO_PYRAMID_ALIGN_H_

#include <math.h>

#include "../Config/Configurable.h"
#include "../Config/ConfigManager.h"
#include "../Sensors/DriveEncoders.h"
#include "../ComponentData/ComponentData.h"
#include "../ComponentData/DrivetrainData.h"

using namespace data;
using namespace data::drivetrain;

namespace APA_State
{
	enum Enum
	{
		DISABLED,
		GO_TO_CENTER,
		WAIT_FOR_SETTLE,
		TURN
	};
};

class AutoPyramidAlign : public Configurable
{
public:
	AutoPyramidAlign();
	~AutoPyramidAlign();
	
	void Tick();
	
	void Start();
	void Reset();
	
	void Configure();
private:
	double m_driveToCenterDistance;
	double m_turnAngle;
	double m_driveSpeed;
	double m_turnSpeed;
	
	double m_startRobotDist;
	double m_startRobotAngle;
	
	int m_waitForSettleTicks;
	int m_timer;
	
	APA_State::Enum m_currentState;
	
	DrivetrainData* m_driveTrain;
	
};

#endif
