#ifndef SMARTERDASHBOARD_H_
#define SMARTERDASHBOARD_H_

#include <WPILib.h>
#include <semLib.h>

#include <queue>
#include <sstream>

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

namespace TelemetryType
{
	enum Enum
	{
		BEGIN = 0,
		CLIMBER_STATE = 0,
		CLIMBER_WINCH_PAWL_CURRENT = 1,
		CLIMBER_WINCH_PAWL_OUTPUT_DUTY_CYCLE = 2,
		CLIMBER_WINCH_PAWL_TIMED_OUT = 3,
		END = 4,
	};
}

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
	
	template <class T> void SetTelemetryData(TelemetryType::Enum type, T data);
	
	void EnqueueMessage(NetBuffer* buff, NetChannel::Enum method, int channel);
	
	void EnqueueShooterMessage(MessageType::Enum header, float time, float value);
	
private:
	static SmarterDashboard* m_instance;
	
	string m_telemetryData[TelemetryType::END - TelemetryType::BEGIN];
	string m_telemetryTypeNames[TelemetryType::END - TelemetryType::BEGIN];
	
	SEM_ID m_queueSem;
	queue<SD_Message> m_netBufferQueue;
	
	NetServer* m_server;
};

#endif
