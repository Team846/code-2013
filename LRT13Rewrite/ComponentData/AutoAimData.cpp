#include "AutoAimData.h"

using namespace data;
using namespace data::autoaim;

AutoAimData::AutoAimData()
{
	errorThreshold = 0;
	desiredX = 0;
	desiredY = 0;
	currentX = 0;
	currentY = 0;
	strength = 0;
}

void AutoAimData::setErrorThreshold(int threshold)
{
	errorThreshold = threshold;
}

void AutoAimData::setDesiredX(int x)
{
	desiredX = x;
}

void AutoAimData::setDesiredY(int y)
{
	desiredY = y;
}

void AutoAimData::setCurrentX(int x)
{
	currentX = x;
}

void AutoAimData::setCurrentY(int y)
{
	currentY = y;
}

void AutoAimData::setStrength(double new_strength)
{
	strength = new_strength;
}

int AutoAimData::getErrorThreshold()
{
	return errorThreshold;
}

int AutoAimData::getDesiredX()
{
	return desiredX;
}

int AutoAimData::getDesiredY()
{
	return desiredY;
}

int AutoAimData::getCurrentX()
{
	return currentX;
}

int AutoAimData::getCurrentY()
{
	return currentY;
}

double AutoAimData::getStrength()
{
	return strength;
}
