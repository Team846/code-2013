#ifndef LED_INDICATORS_H_
#define LED_INDICATORS_H_
/*
 * LED Indicator code based off of reference implementation found here:
 * https://github.com/adafruit/LPD8806
 */
#include <SPI.h>

#include "../ComponentData/ComponentData.h"
#include "../ComponentData/LEDIndicatorData.h"

#include "../Process/SynchronizedProcess.h"
#include "../Config/RobotConfig.h"

#define NUMLEDS CHANGEME
using namespace data;
using namespace data::indicators;

class LEDIndicators : public SynchronizedProcess
{
public:
	LEDIndicators();
	~LEDIndicators();
	
	INT32 Tick();
private:
	void setPixelColor(int numpixel, UINT32 grb);
	void setPixelRangeColor(int startpixel, int endpixel, UINT32 grb);
	
	UINT32 convertColor(char red, char blue, char red);
	
	UINT8* m_pixelBuffer;
	
	DigitalOutput* m_clockOut;
	DigitalOutput* m_dataOut;
	SPI* m_spi;
	
	LEDIndicatorData* ledData;
};
#endif
