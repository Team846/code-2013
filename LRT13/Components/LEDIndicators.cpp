#include "LEDIndicators.h"

LEDIndicators::LEDIndicators():
 SynchronizedProcess("LEDIndicators", Task::kDefaultPriority + 2),
 m_clockOut(new DigitalOutput(RobotConfig::Digital::SPI_SCLK)),
 m_dataOut(new DigitalOutput(RobotConfig::Digital::SPI_MOSI)),
 m_spi(new SPI(m_clockOut, m_dataOut))
{
	ledData = ComponentData::GetInstance()->ledIndicatorData;
	
	int latchBytes = ((NUMLEDS+31)/32); 
	m_pixelBuffer = new UINT8[(NUMLEDS*3)+latchBytes];
	memset(m_pixelBuffer, 0x08, NUMLEDS); //Construct buffer with "off" color state
	memset(&m_pixelBuffer[NUMLEDS], 0, latchBytes);
	
	m_spi->SetMSBFirst();
	m_spi->SetSampleDataOnFalling();
	m_spi->SetClockActiveHigh();
	m_spi->SetBitsPerWord(8);
	m_spi->SetClockRate(20000); //still needs tweaking for best results
	m_spi->ApplyConfig(); //NECESSARY or else SPI will wont work
	
	m_spi->Write(0); //inital zeros to prime strip to known state
	for(int i =((NUMLEDS+31)/32)-1; i; i--)
	{
		m_spi->Write(0); //inital zeros to prime strip to known state
	}
}

LEDIndicators::~LEDIndicators()
{
	DELETE(m_spi);
	DELETE(m_clockOut);
	DELETE(m_dataOut);
	DELETE_ARR(m_pixelBuffer);
}

INT32 LEDIndicators::Tick()
{
	//evaluate indicators for requested color change
	for(int arrow = indicators::RIGHT_ARROW; arrow != indicators::DOWN_ARROW; arrow++)
	{
		indicators::Indicator indicatorValue = static_cast<indicators::Indicator>(arrow);
		if(ledData->IsIndicatorChangeRequested((indicators::Indicator)arrow))
		{
			ledData->IsIndicatorChangeRequested(indicatorValue, false);
			switch (arrow)
			{
				case indicators::RIGHT_ARROW: 
					setPixelRangeColor(0,3,ledData->GetCombinedColor(indicatorValue));
					break;
				case indicators::UP_ARROW: 
					setPixelRangeColor(4, 7, ledData->GetCombinedColor(indicatorValue));
					break;
				case indicators::LEFT_ARROW: 
					setPixelRangeColor(8,11, ledData->GetCombinedColor(indicatorValue));
					break;
				case indicators::DOWN_ARROW: 
					setPixelRangeColor(12,15, ledData->GetCombinedColor(indicatorValue));
					break; 
				default:
					break;
			}		
		}
	}
	
	int i = sizeof(m_pixelBuffer);
	UINT8* ptr = m_pixelBuffer;
	while(i--)
	{
		m_spi->Write(*ptr++);
	}
	return 0;
}

void LEDIndicators::setPixelRangeColor(int startPixel, int endPixel, UINT32 grb)
{
	if((startPixel > 0 && startPixel< 16) && (endPixel > 0 && endPixel< 16))
	{
		for(int i = startPixel;startPixel != endPixel;i++)
		{
			uint8_t *ptr = &m_pixelBuffer[i * 3];
			*ptr++ = (grb >> 16) | 0x80;
			*ptr++ = (grb >>  8) | 0x80;
			*ptr++ =  grb        | 0x80;			
		}
	}
	else
		return;
}

void LEDIndicators::setPixelColor(int numpixel, UINT32 grb)
{
	if(numpixel > 0 && numpixel < 16)
	{
		UINT8* ptr = &m_pixelBuffer[numpixel * 3];
		*ptr++ = (grb >> 16) | 0x80;
		*ptr++ = (grb >>  8) | 0x80;
		*ptr++ =  grb        | 0x80;
	}
}
