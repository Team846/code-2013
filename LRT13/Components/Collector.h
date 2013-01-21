#ifndef COLLECTOR_H_
#define COLLECTOR_H_

#include <string>

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "ComponentWithJaguar.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Jaguar/AsyncCANJaguar.h"

using namespace data;

class Collector : public ComponentWithJaguar, public Configurable, public Loggable
{
public:
	Collector();
	~Collector();
	
	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void whenEnabled();
	virtual void whenDisabled();
	
	virtual void Configure();
	virtual void Log();
private:
	string m_configSection;
	
	float m_dutyCycle;
	
	bool m_isEnabled;
};

#endif
