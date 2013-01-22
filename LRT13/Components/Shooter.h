#ifndef SHOOTER_H_
#define SHOOTER_H_

#include <string>

#include "ComponentWithJaguar.h"
#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Jaguar/AsyncCANJaguar.h"

using namespace data;

/*!
 * @brief Provides control over the shooter component.
 * @author Tony Peng
 */
class Shooter : public ComponentWithJaguar, public Configurable, public Loggable
{
public:
	Shooter();
	~Shooter();
	
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
