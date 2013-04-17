#include "LEDIndicatorData.h"

using namespace data;
using namespace data::indicators;

LEDIndicatorData::LEDIndicatorData()
{
	memset(r, 0, sizeof(r));
	memset(g, 0, sizeof(g));
	memset(b, 0, sizeof(b));
}

void LEDIndicatorData::setColorRGB(INT8 red, INT8 green, INT8 blue, Indicator arrow)
{
	r[arrow] = red;
	g[arrow] = green;
	b[arrow] = blue;
}

INT8 LEDIndicatorData::getColorR(Indicator arrow)
{
	return r[arrow];
}

INT8 LEDIndicatorData::getColorG(Indicator arrow)
{
	return g[arrow];
}

INT8 LEDIndicatorData::getColorB(Indicator arrow)
{
	return b[arrow];
}
