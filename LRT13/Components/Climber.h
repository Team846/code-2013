#ifndef CLIMBER_H_
#define CLIMBER_H_

#include <string>

#include "../ComponentData/RobotData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Jaguar/AsyncCANJaguar.h"
#include "../Pneumatics/Pneumatics.h"
#include "../ComponentData/ClimberData.h"


#include "Component.h"

using namespace data;

/*!
 * @brief Provides control over the climber component.
 * @author Eva Lomazov, Christina Lin, Michelle Chang, Brian Axelrod, Julia Huang
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
	
	Pneumatics* m_pneumatics;
	
	AsyncCANJaguar m_winch_worm;
	double m_winch_current_threshold;
	double m_winch_engage_duty_cycle;
	int m_timer;
	int m_timer_threshold;
	bool m_isEnabled;
	
	
	data::climber::state m_state;
};

#endif
