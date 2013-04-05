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
//	AsyncPrinter::Printf("Alive\n");
	int r = ComponentData::GetInstance()->ledIndicatorData->getColorR();
	int g = ComponentData::GetInstance()->ledIndicatorData->getColorG();
	int b = ComponentData::GetInstance()->ledIndicatorData->getColorB();
		
	for(int i = 0; i < 16; i++)
	{
		spi->Write(r);
		spi->Write(g);
		spi->Write(b);
	}
	spi->Write(0);
	
	taskDelay(sysClkRateGet() / 100);
}
