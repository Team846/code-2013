#ifndef NET_SERVER_TEST_H_
#define NET_TEST_H_

#include <vxWorks.h>
#include <sysLib.h>

#include "UnitTest.h"

#include "../Network/NetConnection.h"
#include "../Network/NetBuffer.h"

using namespace Network;

class NetServerUnitTest : public UnitTest
{
public:
	NetServerUnitTest();
	void Run();
private:
	NetConnection* m_conn;
};

#endif
