#include "LEDIndicators.h"
#include "../Config/RobotConfig.h"

#define NUM_LEDS 16
LEDIndicators::LEDIndicators() :
	AsyncProcess("LED Indicators"), m_clockOut(RobotConfig::Digital::LED_CLOCK_OUT), m_dataOut(RobotConfig::Digital::LED_DATA_OUT)
{
	// get the strip's attention
	writezeros(4);
}

LEDIndicators::~LEDIndicators()
{

}

INT32 LEDIndicators::Tick()
{
	//	AsyncPrinter::Printf("Alive\n");

	uint16_t i;
	Indicator arrow;

	// write 24 bits per pixel
	for (i = 0; i < NUM_LEDS; i++)
	{
		arrow = (Indicator)(i / 4);
		int r = ComponentData::GetInstance()->ledIndicatorData->getColorR(arrow);
		int g = ComponentData::GetInstance()->ledIndicatorData->getColorG(arrow);
		int b = ComponentData::GetInstance()->ledIndicatorData->getColorB(arrow);
		//replace values with
		//g, r, b; currently testing
		//yes, that's the correct order
		write8(b | 0x80);
		write8(r | 0x80);
		write8(g | 0x80);
	}

	// to 'latch' the data, we send just zeros
	writezeros(3 * NUM_LEDS * 2);
}

// Basic, push SPI data out
void LEDIndicators::write8(uint8_t d)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		if (d & (1 << (7 - i)))
			m_dataOut.Set(1);
		else
			m_dataOut.Set(0);
			
		//taskDelay(sysClkRateGet() / 1000);
		m_clockOut.Set(1);
		taskDelay(sysClkRateGet() / 1000);
		m_clockOut.Set(0);
		taskDelay(sysClkRateGet() / 1000);
	}
}

// Basic, push SPI data out
void LEDIndicators::writezeros(uint16_t n)
{
	m_dataOut.Set(0);
	//taskDelay(sysClkRateGet() / 1000);
	for (uint16_t i = 0; i < 8 * n; i++)
	{
		m_clockOut.Set(1);
		//taskDelay(sysClkRateGet() / 1000);
		m_clockOut.Set(0);
		//taskDelay(sysClkRateGet() / 1000);
	}
}
