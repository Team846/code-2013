#ifndef LRT_ENCODER_H_
#define LRT_ENCODER_H_

#include "WPILib.h"
#include "../Log/Loggable.h"
#include "../Utils/AsyncPrinter.h"

/*!
 * @brief LRTEncoder patches bugs in WPILib's Encoder class.
 */
class LRTEncoder: public Encoder, public Loggable
{

public:
	/*!
	 * @brief Constructs an LRT Encoder
	 * @param name name of encoder
	 * @param sourceA channel A
	 * @param sourceB channel B
	 * @param trim scaling factor for the encoder
	 */
	LRTEncoder(const char * name, UINT32 sourceA, UINT32 sourceB,
			double trim = 1.0);

	/*!
	 * @brief destroys this LRT Encoder
	 */
	~LRTEncoder();

	/*!
	 * @brief Starts the LRT Encoder
	 */
	void Start();

	/*!
	 * @brief Gets the current count of the encoder
	 * @return encoder Tick count
	 */
	INT32 Get();

	/*!
	 * @brief Gets the current rate of the encoder
	 * @return encoder Tick rate
	 */
	double GetRate();

	virtual void Log();
private:
	double trim;
	std::string m_name;
	DISALLOW_COPY_AND_ASSIGN(LRTEncoder);
	
};

#endif
