#ifndef COLLECTOR_H_
#define COLLECTOR_H_

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "Component.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Jaguar/AsyncCANJaguar.h"

using namespace data;

class ESC;

class Collector : public Component, public Configurable, public Loggable
{
public:
	Collector();
	~Collector();
	
	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void Update();
	
	virtual void Configure();
	virtual void Log();
private:
	bool m_isEnabled;
	AsyncCANJaguar* m_jaguar;
};

#endif
