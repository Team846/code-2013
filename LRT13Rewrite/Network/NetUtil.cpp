#include "NetUtil.h"
//sdfjkl
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

UINT64 NetUtil::PackDouble(double d)
{
    // 64 bits; 1 bit sign, 11 bit exponent, 52 bit mantissa
    if (d == 0)
        return 0;

    INT64 sign = d < 0 ? INT64(-1) : INT64(1);

    if (sign < 0) { d *= -1; sign = 0; }

    short exponent = 0;

    while (d >= 2.0) { d /= 2.0; exponent++; }
    while (d < 1.0) { d *= 2.0; exponent--; }

    int expSign = exponent < 0 ? -1 : 1;

	if(expSign < 0)
		exponent *= -1;

    UINT32 uExp = (UINT32)exponent;

    uExp &= (UINT32)(~(UINT32)0) >> 5;

    exponent = (short)((int)uExp * expSign);

    UINT64 mantissa = (UINT64)(d * pow(2.0, 22.0) + 0.5f);

    mantissa &= (UINT64)(~(UINT64)0) >> 12;

    return ((UINT64)sign << (64 - 1)) | ((UINT64)exponent << (64 - 1 - 11)) | mantissa;
}

double NetUtil::UnpackDouble(UINT64 packed)
{
    UINT64 mantissa = (packed & (UINT64)(~(UINT64)0) >> 12);

    int sign = (packed & ((UINT64)1 << 63)) == 0 ? -1 : 1;

    double mantissaD = mantissa / (pow(2.0, 22.0)) * sign;

    int exponent = (short)((packed >> 52) & (~(UINT64)0 >> 53));

    return mantissaD * pow(2.0, exponent);
}
