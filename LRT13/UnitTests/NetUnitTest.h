#ifndef NET_TEST_H_
#define NET_TEST_H_

#include <vxWorks.h>
#include <sysLib.h>

#include "UnitTest.h"

#include "../Network/NetConnection.h"
#include "../Network/NetBuffer.h"

using namespace Network;

class NetUnitTest : public UnitTest
{
public:
	NetUnitTest();
	void Run();
private:
	NetConnection* m_conn;
};

#endif
