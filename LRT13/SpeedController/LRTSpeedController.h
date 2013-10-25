#ifndef LRT_SPEEDCONTROLLER_H_
#define LRT_SPEEDCONTROLLER_H_

/*!
 * @brief Interface for various speed controllers
 * 
 * @author Manoj Vasishta
 */

class LRTSpeedController
{
public:
	 typedef enum 
	 { 
		 kNeutralMode_Jumper = 0,
		 kNeutralMode_Brake = 1, 
		 kNeutralMode_Coast = 2
	 } NeutralMode;
	
	virtual const char* GetName() = 0;
	virtual void SetDutyCycle(float pwm) = 0;
	virtual void ConfigNeutralMode(LRTSpeedController::NeutralMode mode) = 0;
};

#endif
