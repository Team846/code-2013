#include "LEDIndicatorData.h"

using namespace data;
using namespace data::indicators;

LEDIndicatorData::LEDIndicatorData()
{
	r = 0;
	g = 0;
	b = 0;
}

void LEDIndicatorData::setColorRGB(uint8_t red, uint8_t green, uint8_t blue)
{
	r = red;
	g = green;
	b = blue;
}

uint8_t LEDIndicatorData::getColorR()
{
	return r;
}

uint8_t LEDIndicatorData::getColorG()
{
	return g;
}

uint8_t LEDIndicatorData::getColorB()
{
	return b;
}
