#include "LEDIndicatorData.h"

using namespace data;
using namespace data::indicators;

LEDIndicatorData::LEDIndicatorData()
{
	r = 0;
	g = 0;
	b = 0;
}

void LEDIndicatorData::setColorRGB(INT8 red, INT8 green, INT8 blue)
{
	r = red;
	g = green;
	b = blue;
}

INT8 LEDIndicatorData::getColorR()
{
	return r;
}

INT8 LEDIndicatorData::getColorG()
{
	return g;
}

INT8 LEDIndicatorData::getColorB()
{
	return b;
}
