#ifndef COLLECTOR_H_
#define COLLECTOR_H_

#include <string>

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/PID.h"
#include "../ComponentData/StorageData.h"
#include "../Jaguar/AsyncCANJaguar.h"
#include "../Components/Component.h"
#include "../ComponentData/RobotData.h"

using namespace data;

/*!
 * @brief Provides control over the collector component.
 * @author Raphael Chang
 */
class Storage : public Component, public Configurable, public Loggable
{
public:
	Storage();
	~Storage();
	
	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void enabledPeriodic();
	virtual void disabledPeriodic();
	
	virtual void Configure();
	virtual void Log();
private:
	AsyncCANJaguar* m_jaguar;
	
	double m_dutyCycle;
	
	string m_configSection;
};

#endif
