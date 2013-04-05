#ifndef SMARTERDASHBOARD_H_
#define SMARTERDASHBOARD_H_

#include <WPILib.h>
#include <semLib.h>

#include <queue>

#include "../Network/NetServer.h"
#include "../Network/NetBuffer.h"
#include "../Network/NetChannel.h"
#include "../Network/MessageType.h"

typedef struct
{
	NetBuffer* nb;
	NetChannel::Enum method;
	int channel;
} SD_Message;

class SmarterDashboard
{
public:
	static SmarterDashboard* Instance();
	static void Start();
	static void Close();
	
	SmarterDashboard();
	~SmarterDashboard();
	
	void Tick();
	void Flush();
	
	void EnqueueMessage(NetBuffer* buff, NetChannel::Enum method, int channel);
	
private:
	static SmarterDashboard* m_instance;
	
	SEM_ID m_queueSem;
	queue<SD_Message> m_netBufferQueue;
	
	NetServer* m_server;
};

#endif
