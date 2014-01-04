#ifndef DRIVE_ESC_H_
#define DRIVE_ESC_H_

#include <WPILib.h>
#include <utility>
#include <string>

#include "LRTSpeedController.h"
#include "../Sensors/LRTEncoder.h"
#include "../Utils/AsyncPrinter.h"
#include "../Logging/Loggable.h"

using namespace std;

/*!
 * @brief Container for multiple speed controllers for the drivetrain. Features dithered braking and current limiting.
 */
class DriveESC : public Loggable
{
public:

	/*!
	 * @brief Constructs a new drivetrain ESC object with one motor. 
	 * @param esc the LRTSpeedController
	 * @param encoder the encoder that measures the speed of this ESC
	 * @param name the name of this ESC
	 */
	DriveESC(LRTSpeedController *esc, LRTEncoder *encoder, string name);
	
	/*!
	 * @brief Constructs a new drivetrain ESC object with two motors. 
	 * @param esc1 the first LRTSpeedController
	 * @param esc2 the second LRTSpeedController 
	 * @param encoder the encoder that measures the speed of this ESC
	 * @param name the name of this ESC
	 */
	DriveESC(LRTSpeedController *esc1, LRTSpeedController *esc2, LRTEncoder *encoder, string name);

	
	/*!
	 * @brief Cleans up the speed controller container.
	 */
	~DriveESC();

	/*!
	 * @brief Sets the duty cycle. Includes dithered braking to ensure a linear response. 
	 * @param dutycycle the duty cycle to set
	 */
	void SetDutyCycle(float dutycycle);

	/*!
	 * @brief Sets the acceleration current limit. 
	 * @param limit the current limit in %
	 */
	void SetForwardCurrentLimit(float limit);
	
	/*!
	 * @brief Sets the reverse direction current limit.
	 * @param limit the current limit in %
	 */
	void SetReverseCurrentLimit(float limit);
	
	/*!
	 * @brief Resets the caching.
	 */
	void ResetCache();

	void Disable();
	
	void Log();
	
private:
	struct BrakeAndDutyCycle
	{
		float dutyCycle;
		float braking;
	};
	
	float DitheredBraking(float dutyCycle, float speed);
	float CurrentLimit(float dutyCycle, float speed);
	BrakeAndDutyCycle CalculateBrakeAndDutyCycle(float target_speed, float current_speed);

	LRTEncoder* m_encoder;
	LRTSpeedController *m_controller1, *m_controller2;
	
	float m_dutyCycle;
	
	int m_cycle_count;
	
	float m_forwardCurrentLimit; // % stall current for acceleration
	float m_reverseCurrentLimit; // % stall current for reversing direction

	bool m_shouldBrakeThisCycle;

};

#endif /* ESC_H_ */
