#include "LEDIndicatorData.h"

using namespace data;
using namespace data::indicators;

LEDIndicatorData::LEDIndicatorData()
{
	m_rgb = 0;
}

void LEDIndicatorData::setColorRGB(Indicator i, INT8 red, INT8 green, INT8 blue)
{
	m_rgb = 0;
	
	m_rgb = ((INT32)red << 24) | ((INT32)green << 16) | ((INT32)blue << 8);
}
			
int LEDIndicatorData::getColor(Indicator i)
{
	return m_rgb;
}

int LEDIndicatorData::getColorR(Indicator i)
{
	return m_rgb >> 24;
}

int LEDIndicatorData::getColorG(Indicator i)
{
	return (m_rgb >> 16) & 0xff;
}

int LEDIndicatorData::getColorB(Indicator i)
{
	return (m_rgb >> 8) & 0xff;
}
