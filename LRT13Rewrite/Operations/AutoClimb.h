#ifndef AUTOCLIMB_H_
#define AUTOCLIMB_H_

#include "../Config/Configurable.h"
#include "../Components/Climber.h"
#include "../ComponentData/ClimberData.h"
#include "../ComponentData/ShooterData.h"
#include "../ComponentData/ComponentData.h"
#include "../Process/AsyncProcess.h"
#include "../Sensors/IMU.h"

using namespace data;
using namespace data::climber;

typedef enum
{
	USER_ABORT = 0x10,
	COMMUNICATION_ERROR = 0x20,
	WINCH_PAWL_TIMEOUT = 0x30,
	WINCH_PAWL_OVER_CURRENT = 0x40,
} ABORT_CODE;

typedef enum
{
	ABORT_NEXT_LEVEL = 0x00, // don't attempt to continue the climb
	LOCKDOWN = 0x10, // brace for impact!
} ABORT_MODE;

class AutoClimb : public AsyncProcess, Configurable
{
public:
	static AutoClimb* Instance();
	
	AutoClimb();
	~AutoClimb();
	
	INT32 Tick();
	
	void Configure();
	
	void Reset();
	
	void ClimbOneLevel(bool force=false);
	
	bool CanAbort();
	void UserAbort(bool force=false, ABORT_MODE severity=ABORT_NEXT_LEVEL);
private:
	static AutoClimb* m_instance;
		
	void AbortClimb(ABORT_CODE errorCode, bool force=false, ABORT_MODE severity=ABORT_NEXT_LEVEL);
	
	bool m_canAbort;
	bool m_aborted;
	
	bool m_climbRequested;
	bool m_climbForce;
	
	int m_climbLevel;
	
	double m_winchPawlAbortThreshold;
	double m_winchPawlClimbThreshold;

	ComponentData* m_componentData;
	ClimberData* m_climber;
	IMU* m_imu;
};

#endif
