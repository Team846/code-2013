#ifndef COLLECTOR_H_
#define COLLECTOR_H_

#include <string>

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/PID.h"
#include "../ComponentData/CollectorData.h"
#include "../Jaguar/AsyncCANJaguar.h"
#include "../Components/Component.h"
#include "DigitalInput.h"
#include "../ComponentData/RobotData.h"

using namespace data;

/*!
 * @brief Provides control over the collector component.
 * @author Raphael Chang, Tony Peng, Manoj Vasishta
 */
class Collector : public Component, public Configurable, public Loggable
{
public:
	Collector();
	~Collector();
	
	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void enabledPeriodic();
	virtual void disabledPeriodic();
	
	virtual void Configure();
	virtual void Log();
private:
	AsyncCANJaguar* m_jaguar;
	
	// Proximity Sensors
	DigitalInput* m_proximity;
	
	// Frisbee Counters
	int m_count;
	int m_samplesThreshold;
	
	float m_dutyCycle;
	
	string m_configSection;
};

#endif
