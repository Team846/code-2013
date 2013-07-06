#ifndef LED_INDICATORS_H_
#define LED_INDICATORS_H_

#include <SPI.h>
#include "../Process/SynchronizedProcess.h"
#include "../Config/RobotConfig.h"

class LEDIndicators : public SynchronizedProcess
{
public:
	LEDIndicators();
	~LEDIndicators();
	void SetPixelColor(int numPixel, int r, int g, int b);
	void SetPixelRange(int startPixel, int endPixel, int r, int g, int b); //start and end values are inclusive of the range -MV
	
	INT32 Tick();
private:
	UINT8* m_pixelBuffer;
	DigitalOutput* m_clockOut;
	DigitalOutput* m_dataOut;
	SPI* m_spi;
};
#endif
