#include "LEDIndicators.h"
#include "../Config/RobotConfig.h"

LEDIndicators::LEDIndicators() :
	AsyncProcess("LED Indicators"), m_clockOut(RobotConfig::Digital::LED_CLOCK_OUT), m_dataOut(RobotConfig::Digital::LED_DATA_OUT)
{
	numLEDs = 16;

}

LEDIndicators::~LEDIndicators()
{

}

INT32 LEDIndicators::Tick()
{
	//	AsyncPrinter::Printf("Alive\n");
	int r = ComponentData::GetInstance()->ledIndicatorData->getColorR();
	int g = ComponentData::GetInstance()->ledIndicatorData->getColorG();
	int b = ComponentData::GetInstance()->ledIndicatorData->getColorB();

	uint16_t i;

	// get the strip's attention
	writezeros(4);

	// write 24 bits per pixel
	for (i = 0; i < numLEDs; i++)
	{
		write8(r | 0x80);
		write8(g | 0x80);
		write8(b | 0x80);
	}

	// to 'latch' the data, we send just zeros
	writezeros(3 * numLEDs * 2);

	taskDelay(sysClkRateGet() / 100);
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
	for (uint16_t i = 0; i < 8 * n; i++)
	{
		m_clockOut.Set(1);
		taskDelay(sysClkRateGet() / 1000);
		m_clockOut.Set(0);
		taskDelay(sysClkRateGet() / 1000);
	}
}
