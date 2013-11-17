#ifndef DISCLOCATOR_H_
#define DISCLOCATOR_H_

#include "../Network/NetBuffer.h"
#include "../Network/NetPeer.h"
#include "../Utils/Util.h"

/*!
 * @brief Interfaces with the disc locator on the offboard processor.
 * 
 * @author Tony Peng
 */
class DiscLocator
{
public:
	DiscLocator();
	~DiscLocator();
	
	void Update();
	
private:
	NetPeer* m_conn;
};

#endif
