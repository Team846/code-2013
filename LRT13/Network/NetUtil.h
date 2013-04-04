#ifndef NETUTIL_H_
#define NETUTIL_H_

#include <WPILib.h>
#include <cmath>

namespace Network
{
	class NetUtil
	{
	public:
		static UINT32 PackFloat(float f);

        static float UnpackFloat(UINT32 packed);
	};
};

#endif