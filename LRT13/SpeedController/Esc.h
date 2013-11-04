#ifndef ESC_H_
#define ESC_H_

#include <WPILib.h>
#include <utility>
#include <string>

#include "AsyncCANJaguar.h"
#include "LRTTalon.h"
#include "..\Config\ConfigManager.h"
#include "..\Config\Configurable.h"
#include "..\Utils\AsyncPrinter.h"
#include "..\Sensors\DriveEncoders.h"
#include "..\Utils\RunningSum.h"

#define TALON 0

using namespace std;

class ESC : public Configurable
{
public:

	/*!
	 * @brief Constructs a new LRT specific ESC object. 
	 * @param channel Port of the Talon
	 * @param brakeChannel Port of the Talon jumper
	 * @param encoder 
	 * @param name
	 */
	ESC(int channel, LRTEncoder* encoder, string name, int brakeChannel);
	/*!
	 * @brief Constructs a new LRT specific ESC object. 
	 * @param channelA Port of the first Talon
	 * @param channelB Port of the second Talon 
	 * @param brakeChannelA Port of the first Talon jumper
	 * @param brakeChannelB Port of the second Talon jumper
	 * @param encoder
	 * @param name
	 */
	ESC(int channelA, int channelB, int brakeChannelA, int brakeChannelB, LRTEncoder* encoder, string name);

	/*!
	 * @brief Constructs a new LRT specific ESC object. 
	 * @param channel CANID of the Jaguar 
	 * @param encoder 
	 * @param name
	 */
	ESC(int channel, LRTEncoder* encoder, string name);
	/*!
	 * @brief Constructs a new LRT specific ESC object. 
	 * @param channelA CANID of first Jaguar
	 * @param channelB CANID of second Jaguar
	 * @param encoder
	 * @param name
	 */
	ESC(int channelA, int channelB, LRTEncoder* encoder, string name);

	
	/*!
	 * @brief Cleans up the speed controller including cleanup of the jaguar resources.
	 */
	~ESC();

	/*!
	 * @brief loads in mutable parameters of the ESC
	 */
	virtual void Configure();

	/*!
	 * @brief Sets the duty cycle. Includes break dithering to ensure a linear response. 
	 * @param dutycycle
	 */
	void SetDutyCycle(float dutycycle);

	float DitheredBraking(float dutyCycle, float speed);
	float CurrentLimit(float dutyCycle, float speed);
	/*!
	 * @brief Resets the caching.
	 */
	void ResetCache();
	
	void DecrementMaxVDiff();
	void IncrementMaxVDiff();
	void ResetMaxVDiff();

	void Disable();
	LRTEncoder* m_encoder;
private:

	LRTSpeedController *m_controller1, *m_controller2;
	std::string namea, nameb;
	
	float maxVDiff;
	//	CANJaguar *m_jag1, *m_jag2;

	float m_delta_voltage_limit;
	
	string m_name;
	int m_cycle_count;

	struct brakeAndDutyCycle
	{
		float dutyCycle;
		float braking;
	};

	brakeAndDutyCycle CalculateBrakeAndDutyCycle(float target_speed,
			float current_speed);
};

#endif /* ESC_H_ */
