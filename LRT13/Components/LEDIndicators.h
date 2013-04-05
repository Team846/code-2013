#ifndef LED_INDICATOR_H_
#define LED_INDICATOR_H_

#include <WPILib.h>
#include <SPI.h>
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
	
private:
	DigitalOutput m_clockOut;
	DigitalOutput m_dataOut;
	SPI* spi;
};

#endif
