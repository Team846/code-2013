#ifndef COLLECTOR_H_
#define COLLECTOR_H_

#include <string>

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Jaguar/AsyncCANJaguar.h"
#include "../Components/Component.h"
#include "DigitalInput.h"
#include "../ComponentData/RobotData.h"

#define SAMPLES_THRESHOLD 4 //needs tweaking


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
	
	//Proximity Sensors
	DigitalInput* m_proximityA; 
	DigitalInput* m_proximityB;
	
	//Frisbee Counters
	int m_upCount;
	int m_downCount;
	int m_errCount;
	
	string m_configSection;
	float m_dutyCycle;
	
	bool m_isEnabled;
};

#endif
