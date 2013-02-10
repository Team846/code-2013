#ifndef CLIMBER_H_
#define CLIMBER_H_

#include <string>

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Jaguar/AsyncCANJaguar.h"
#include "Component.h"

using namespace data;

/*!
 * @brief Provides control over the climber component.
 * @author Eva Lomazov, Christina Lin, Michelle Chang, Brian Axelrod
 */
class Climber : public Component, public Configurable, public Loggable
{
public:
	Climber();
	~Climber();
	
	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void enabledPeriodic();
	virtual void disabledPeriodic();
	
	virtual void Configure();
	virtual void Log();
private:
	string m_configSection;
	
	bool m_isEnabled;
};

#endif
