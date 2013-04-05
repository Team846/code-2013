#include "LEDIndicatorData.h"

using namespace data;
using namespace data::indicators;

LEDIndicatorData::LEDIndicatorData()
{
	m_rgb = 0;
}

void LEDIndicatorData::setColorRGB(INT8 red, INT8 green, INT8 blue)
{
	m_rgb = 0;
	
	m_rgb = ((INT32)red << 24) | ((INT32)green << 16) | ((INT32)blue << 8);
}
			
int LEDIndicatorData::getColor()
{
	return m_rgb;
}

int LEDIndicatorData::getColorR()
{
	return m_rgb >> 24;
}

int LEDIndicatorData::getColorG()
{
	return (m_rgb >> 16) & 0xff;
}

int LEDIndicatorData::getColorB()
{
	return (m_rgb >> 8) & 0xff;
}
