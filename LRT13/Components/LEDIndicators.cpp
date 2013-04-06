#include "LEDIndicators.h"

LEDIndicators::LEDIndicators() :
	AsyncProcess("LED Indicators"), m_clockOut(8), m_dataOut(13), spi(new SPI(m_clockOut, m_dataOut))
{
	numLEDs = 16;

}

LEDIndicators::~LEDIndicators() {
	
}

INT32 LEDIndicators::Tick() {
//	AsyncPrinter::Printf("Alive\n");
	int g = ComponentData::GetInstance()->ledIndicatorData->getColorG();
	int r = ComponentData::GetInstance()->ledIndicatorData->getColorR();
	int b = ComponentData::GetInstance()->ledIndicatorData->getColorB();
	
	  uint16_t i;
  
	// get the strip's attention
	writezeros(4);

	// write 24 bits per pixel
	for (i=0; i<numLEDs; i++ ) {
		write8(g); //chinese led strips like to 
		write8(r); //use grb instead of rgb for
		write8(b); //some reason >_>
	}
  
	// to 'latch' the data, we send just zeros
	writezeros(3*numLEDs*2);
	
	taskDelay(sysClkRateGet() / 100);
}


// Basic, push SPI data out
void LEDIndicators::write8(uint8_t d) {
  for (uint8_t i=0; i<8; i++) {
     if (d & _BV(7-i))
       m_dataOut->Write(HIGH);
     else
       m_dataOut->Write(LOW);
     m_clockOut->Write(HIGH);
     m_clockOut->Write(LOW); 
  }
}

// Basic, push SPI data out
void LEDIndicators::writezeros(uint16_t n) {
  m_dataOut->Write(LOW);
  for (uint16_t i=0; i<8*n; i++) {
     m_clockOut->Write(HIGH);
     m_clockOut->Write(LOW); 
}