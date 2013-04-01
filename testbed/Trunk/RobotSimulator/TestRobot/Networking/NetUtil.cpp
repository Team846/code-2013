#include "NetUtil.h"

using namespace Network;

UINT32 NetUtil::PackFloat(float f)
{
	if(f == 0)
		return 0;

    int sign = f < 0 ? -1 : 1;

    if (sign < 0) { f *= -1; sign = 0; }

    char exponent = 0;

    while (f >= 2.0f) { f /= 2.0f; exponent++; }
    while (f < 1.0f) { f *= 2.0f; exponent--; }

    UINT32 mantissa = (UINT32)(f * pow(2.0f, 22.0f) + 0.5f);

    mantissa &= (UINT32)(~(UINT32)0) >> 9;

    return ((UINT32)sign << (32 - 1)) | ((UINT32)exponent << (32 - 1 - 8)) | mantissa;
}

float NetUtil::UnpackFloat(UINT32 packed)
{
    UINT32 mantissa = (packed & (UINT32)(~(UINT32)0) >> 9);

    int sign = (packed & ((UINT32)1 << 31)) == 0 ? -1 : 1;

    float mantissaF = mantissa / (float)(pow(2.0f, 22.0f)) * sign;

    int exponent = (signed char)((packed >> 23) & (~(UINT32)0 >> 24));

    return mantissaF * (float)pow(2.0f, exponent);
}