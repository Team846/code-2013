#ifndef LIVE_NETWORK_SENDABLE_H_
#define LIVE_NETWORK_SENDABLE_H_

#include "LiveNetworkSender.h"
#include <string>
#include <vector>

using namespace std;

/*!
 * @brief Base class for classes that can log to the global log file.
 */
class LiveNetworkSendable
{
public:
	LiveNetworkSendable();
	virtual ~LiveNetworkSendable();
	
	/*!
	 * @brief Sends data to the LiveNetworkSender.
	 */
	virtual void Send() = 0;
	
protected:

	/*!
	 * @brief Sends a value to the network through LiveNetworkSender.
	 * @param value value to log
	 * @param key name of the field
	 * @param table NetworkTable to send to
	 */
	template<typename T> void LiveNetworkSendable::SendToNetwork(T value, string key, string table)
	{
		m_sender->Send(value, key, table);
	}

private:
	LiveNetworkSender *m_sender;
};

#endif /* LIVE_NETWORK_SENDABLE_H_ */
