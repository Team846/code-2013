#ifndef ESC_H_
#define ESC_H_

#include "AsyncCANJaguar.h"
#include "WPILib.h"
#include "..\Config\ConfigManager.h"
#include "..\Config\Configurable.h"
#include "..\Utils\AsyncPrinter.h"
#include "..\Sensors\DriveEncoders.h"
#include "..\Utils\RunningSum.h"
#include <utility>
#include <string>
using namespace std;

class ESC: public Configurable
{
public:
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

	/*!
	 * @brief Resets the caching.
	 */
	void ResetCache();

	void Disable();
private:
	AsyncCANJaguar *m_jag1, *m_jag2;
	std::string namea, nameb;
	//	CANJaguar *m_jag1, *m_jag2;

	LRTEncoder* m_encoder;

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
