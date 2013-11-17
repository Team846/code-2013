#include "LEDIndicatorData.h"

using namespace data;
using namespace data::indicators;

LEDIndicatorData::LEDIndicatorData()
{
	memset(m_indicatorColor, 0, sizeof(m_indicatorColor));
	memset(m_changeRequested, false, sizeof(m_changeRequested));
}

bool LEDIndicatorData::IsIndicatorChangeRequested(Indicator arrow)
{
	return m_changeRequested[arrow];
}

void LEDIndicatorData::IsIndicatorChangeRequested(Indicator arrow, bool value)
{
	m_changeRequested[arrow] = value;
}

void LEDIndicatorData::SetIndicatorColor(unsigned char red, unsigned char green, unsigned char blue, Indicator arrow)
{
	UINT32 combinedColor = ((UINT32) (green | 0x08) << 16) | //grb is actually the correct format for colors, don't change
						   ((UINT32) (red | 0x08) << 8)  |   
							         (blue | 0x08);	
	m_indicatorColor[arrow] = combinedColor;
	m_changeRequested[arrow] = true;
}

char LEDIndicatorData::GetColorR(Indicator arrow)
{
	return (UINT8) (m_indicatorColor[arrow] >> 8) & (UINT32) 255;
}

char LEDIndicatorData::GetColorG(Indicator arrow)
{
	return (char) m_indicatorColor[arrow] >> 16;
}

char LEDIndicatorData::GetColorB(Indicator arrow)
{
	return (char) m_indicatorColor[arrow] & (UINT32) 255;
}

UINT32 LEDIndicatorData::GetCombinedColor(Indicator arrow)
{
	return m_indicatorColor[arrow];
}

