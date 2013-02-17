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
#include "../Sensors/DriveEncoders.h"
#include "../Servo/LRTServo.h"


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
	int m_side_engaged;
	DigitalInput m_digital_input_left, m_digital_input_right;
	bool m_hasStartedEngaging;
	bool m_isEnabled;
	double m_drive_train_position;
	double  m_drive_train_position_threshold;
	DriveEncoders* m_driving_encoders;
	LRTServo m_servo_left;
	LRTServo m_servo_right;
	int m_servo_left_engaged_position;
	int m_servo_right_engaged_position;
	int m_servo_left_disengaged_position;
	int m_servo_right_disengaged_position;
	int m_disengageTimer_threshold;
	GearTooth m_winch_gear_tooth;
	int m_gear_tooth_ticks_position;
	int m_gear_tooth_threshold;
	int m_climbing_level;
	data::climber::state m_previous_state;
	bool m_paused;
	
	data::climber::state m_state;
};

#endif
