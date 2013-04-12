#ifndef LED_INDICATOR_H_
#define LED_INDICATOR_H_

#include <DigitalOutput.h>

#include "../ComponentData/ComponentData.h"
#include "../ComponentData/LEDIndicatorData.h"
#include "../Process/AsyncProcess.h"

using namespace data;
using namespace data::indicators;

class LEDIndicators : public AsyncProcess
{
public:
	LEDIndicators();
	~LEDIndicators();
	
	INT32 Tick();
	void write8(uint8_t d);
	void writezeros(uint16_t n);
	
private:
	int numLEDs;
	DigitalOutput m_clockOut;
	DigitalOutput m_dataOut;
};

#endif