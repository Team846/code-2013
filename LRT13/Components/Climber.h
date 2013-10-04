#ifndef CLIMBER_H_
#define CLIMBER_H_

#include <string>

#include "../ComponentData/ShooterData.h"
#include "../ComponentData/RobotData.h"
#include "../ComponentData/WinchPawlData.h"
#include "../ComponentData/ClimberData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/PID.h"
#include "../ComponentData/DrivetrainData.h"
#include "../SpeedController/AsyncCANJaguar.h"
#include "../Pneumatics/Pneumatics.h"
#include "../ComponentData/ClimberData.h"
#include "../Sensors/DriveEncoders.h"
#include "../Servo/LRTServo.h"
#include "../Utils/SmarterDashboard.h"
#include "../Network/MessageType.h"
#include "../Network/NetChannel.h"
#include "../Utils/Profiler.h"
#include "../Utils/LCD.h"
#include "AnalogChannel.h"

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
	void disengagePTO(bool force=false);
	void engagePTO(bool force=false);
	
	void winchPawlUp(bool fast=true);
	void winchPawlDown(bool fast=true);
	void winchPawlOff();
	
	bool m_ptoEngaged;
	
	string m_configSection;
	
	Pneumatics* m_climberArms;
	Pneumatics* m_hooks;

	double m_winch_current_threshold;
	double m_winch_engage_duty_cycle;
	int m_timer;
	int m_timer_threshold;
	int m_side_engaged;
	DigitalInput m_digital_input_left, m_digital_input_right;
	bool m_hasStartedEngaging;
	bool m_isEnabled;
	int m_drive_train_position;
	int  m_drive_train_position_threshold;
	DriveEncoders* m_driving_encoders;
	LRTServo m_servo_left;
	LRTServo m_servo_right;
	int m_servo_left_engaged_position;
	int m_servo_right_engaged_position;
	int m_servo_left_disengaged_position;
	int m_servo_right_disengaged_position;
	int m_disengageTimer_threshold;
	GearTooth m_winch_gear_tooth;
	AnalogChannel *m_pressure_a;
	AnalogChannel *m_pressure_b;
	NetworkTable *table;
	double m_pressure_scale;
	int m_gear_tooth_ticks_position;
	int m_gear_tooth_threshold;			
	data::climber::state m_previous_state;
	bool m_paused;
	double m_driveSpeed;
	int m_armUpTicks;
	int m_climbingLevelGearToothTicks[3];
	
	ofstream m_logFile;
	
	const static int m_winchPawlUpDirection = 1;
	const static int m_winchPawlDownDirection = -1;
	
	data::climber::state m_state;
	data::climber::state m_waitGoToState; // when the operator wishes to skip to some state
	
	data::shooter::ShooterData* m_shooterData;
	data::climber::WinchPawlData* m_winchPawl;
	data::climber::ClimberData* m_climberData;
};

#endif
