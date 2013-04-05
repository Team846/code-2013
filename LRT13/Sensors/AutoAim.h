#ifndef AUTOAIM_H_
#define AUTOAIM_H_

#include "../ComponentData/ComponentData.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Config/ConfigManager.h"
#include "../Config/Configurable.h"
#include "../Network/NetBuffer.h"
#include "../Network/NetClient.h"
#include "../Network/MessageType.h"
#include "../Utils/Util.h"
#include "../Utils/SmarterDashboard.h"

/*!
 * @brief Interfaces with the auto-aim sensor on the offboard processor
 * 
 * @author Tony Peng
 */
class AutoAim : public Configurable
{
public:
	AutoAim();
	~AutoAim();
	
	void Configure();
	
	void Update();
	
private:
	NetClient* m_client;
	
	int m_maxError;
};

#endif
