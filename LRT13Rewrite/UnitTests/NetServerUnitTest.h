#ifndef NET_SERVER_TEST_H_
#define NET_TEST_H_

#include <cmath>

#include <vxWorks.h>
#include <sysLib.h>

#include "UnitTest.h"

#include "../Network/NetServer.h"
#include "../Network/NetPeer.h"
#include "../Network/NetBuffer.h"
#include "../Network/MessageType.h"

using namespace Network;

class NetServerUnitTest : public UnitTest
{
public:
	NetServerUnitTest();
	void Run();
private:
	NetServer* m_conn;
};

#endif
