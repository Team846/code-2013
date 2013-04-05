#include "LEDIndicators.h"

LEDIndicators::LEDIndicators() :
	AsyncProcess("LED Indicators"), m_clockOut(8), m_dataOut(13), spi(new SPI(m_clockOut, m_dataOut))
{
	spi->SetMSBFirst();
	spi->SetClockRate(2000000);
	spi->Write(0);
}

LEDIndicators::~LEDIndicators() {
	
}

INT32 LEDIndicators::Tick() {
	int color = ComponentData::GetInstance()->ledIndicatorData->getColor();
	
	spi->Write(color);
	
	Wait(0.01);
}
