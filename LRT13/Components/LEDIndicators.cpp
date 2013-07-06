#include "LEDIndicators.h"

LEDIndicators::LEDIndicators():
 SynchronizedProcess("LEDIndicators"),
 m_clockOut(new DigitalOutput(1)),
 m_dataOut(new DigitalOutput(1)),
 m_spi(new SPI(m_clockOut, m_dataOut))
{
	m_spi->SetMSBFirst();
	m_spi->SetSampleDataOnFalling();
	m_spi->SetClockActiveHigh();
	m_spi->SetClockRate(20000); //TODO: needs testing
	m_spi->ApplyConfig(); //NECESSARY or else SPI will wont work
}

LEDIndicators::~LEDIndicators()
{
	DELETE(m_spi);
	DELETE(m_clockOut);
	DELETE(m_dataOut);
}
