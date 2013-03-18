#ifndef AUTOAIM_H_
#define AUTOAIM_H_

#include "../Network/NetBuffer.h"
#include "../Network/NetPeer.h"
#include "../Utils/Util.h"

/*!
 * @brief Interfaces with the auto-aim sensor on the offboard processor
 * 
 * @author Tony Peng
 */
class AutoAim
{
public:
	AutoAim();
	~AutoAim();
	
	void Update();
	
private:
	NetPeer* m_conn;
};

#endif
