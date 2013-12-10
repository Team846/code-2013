#ifndef COLLECTOR_ROLLERS_DATA_H_
#define COLLECTOR_ROLLERS_DATA_H_

#include "ComponentData.h"

class CollectorRollersData : public ComponentData
{
public:
	enum Direction
	{
		FORWARD,
		REVERSE
	};
	
	CollectorRollersData();
	static CollectorRollersData* Get();
	void ResetCommands();
	void Log();
	
	void SetDirection(Direction direction);
	void SetRunning(bool run);
	
	Direction GetDirection();
	bool IsRunning();
	
private:
	bool m_running;
	Direction m_direction;
};

#endif
