#ifndef NETUTIL_H_
#define NETUTIL_H_

#include "Includes.h"
#include <cmath>

namespace Network
{
	class NetUtil
	{
	public:
		static UINT32 PackFloat(float f);
        static float UnpackFloat(UINT32 packed);

		static UINT64 PackDouble(double d);
		static double UnpackDouble(UINT64 packed);
	};
};

#endif