#ifndef WINCH_PAWL_H_
#define WINCH_PAWL_H_

#include <string>

#include "../ComponentData/ComponentData.h"
#include "../ComponentData/WinchPawlData.h"
#include "../SpeedController/AsyncCANJaguar.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Stopwatch.h"
#include "../Utils/SmarterDashboard.h"
#include "../Config/ConfigManager.h"
#include "../Config/Configurable.h"
#include "Component.h"

using namespace data;
using namespace data::climber;

class WinchPawl : public Component, Configurable
{
public:
	WinchPawl();
	~WinchPawl();

	void enabledPeriodic();
	void disabledPeriodic();
	
	void onEnable();
	void onDisable();
	
	void Configure();
	
private:
	Stopwatch m_stopWatch;
	
	AsyncCANJaguar m_jaguar;
	
	string m_configSection;
	
	ComponentData* m_componentData;
	WinchPawlData* m_winchPawlData;
	
	bool m_timedOut;
	
	float m_lastRequestedDutyCycle;
	
	double m_overCurrentThreshold;
	
	int m_overCurrentCounter;
	int m_maxOverCurrentCounter;
	
	bool m_lastEnabled;
	double m_timeout;
};

#endif
