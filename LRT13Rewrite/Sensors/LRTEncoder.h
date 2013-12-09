#ifndef LRT_ENCODER_H_
#define LRT_ENCODER_H_

#include "WPILib.h"
#include "../Logging/Loggable.h"
#include "../Utils/AsyncPrinter.h"

/*!
 * @brief LRTEncoder patches bugs in WPILib's Encoder class.
 */
class LRTEncoder: public Encoder, public Loggable
{
public:
	/*!
	 * @brief Constructs an LRTEncoder
	 * @param name name of encoder
	 * @param sourceA channel A
	 * @param sourceB channel B
	 * @param trim scaling factor for the encoder
	 */
	LRTEncoder(const char * name, UINT32 sourceA, UINT32 sourceB, double trim = 1.0);

	/*!
	 * @brief Destroys this LRTEncoder
	 */
	~LRTEncoder();

	/*!
	 * @brief Starts the LRTEncoder
	 */
	void Start();

	/*!
	 * @brief Gets the current count of the encoder
	 * @return encoder tick count
	 */
	INT32 Get();

	/*!
	 * @brief Gets the current rate of the encoder
	 * @return encoder tick rate
	 */
	double GetRate();

	void Log();
	
private:
	double trim;
	DISALLOW_COPY_AND_ASSIGN(LRTEncoder);
};

#endif
