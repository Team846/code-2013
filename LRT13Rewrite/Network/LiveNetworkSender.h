#ifndef LIVE_NETWORK_SENDER_H_
#define LIVE_NETWORK_SENDER_H_

#include <WPILib.h>
#include <vector>
#include <typeinfo>
#include <map>

class LiveNetworkSendable;

using namespace std;

/*!
 * @brief Logs data from Loggable classes to a global log file in binary format each cycle.
 */
class LiveNetworkSender
{
public:
	static LiveNetworkSender* Instance();
	static void Initialize();
	static void Finalize();

	~LiveNetworkSender();
	
	/*!
	 * @brief Sends data from all objects to the network.
	 */
	void Run();

	/*!
	 * @brief Sends data to the network.
	 * @param value the value to send
	 */
	template<typename T> void Send(T value, string key, string table);

	/*!
	 * @brief Registers a LiveNetworkSendable object for sending.
	 * @param liveNetworkSendable the LiveNetworkSendable object to register
	 */
	static void RegisterLiveNetworkSendable(LiveNetworkSendable* liveNetworkSendable);
	
private:
	LiveNetworkSender();
	
	static LiveNetworkSender *m_instance;

	static vector<LiveNetworkSendable*> liveNetworkSendables;

	map<string, NetworkTable*> tables;
};

#endif /* LIVE_NETWORK_SENDER_H_ */
