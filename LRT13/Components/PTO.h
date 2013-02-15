#ifndef PTO_H_
#define PTO_H_

#include <Servo.h>

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "Component.h"
#include "../ComponentData/PTOData.h"

using namespace data;

/*!
 * @brief Provides control over the PTO component.
 * @author Raphael Chang
 */
class PTO : public Component, public Configurable, public Loggable
{
public:
	PTO();
	~PTO();
	
	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void enabledPeriodic();
	virtual void disabledPeriodic();
	
	virtual void Configure();
	virtual void Log();
private:
	Servo* m_leftServo;
	Servo* m_rightServo;
	string m_configSection;
	int m_leftEngagedServoValue;
    int m_leftDisengagedServoValue;
    int m_rightDisengagedServoValue;
    int m_rightEngagedServoValue;
};

#endif
