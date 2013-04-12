#ifndef AUTOAIM_H_
#define AUTOAIM_H_

#include "../ComponentData/ComponentData.h"
#include "../ComponentData/AutoAimData.h"
#include "../Config/ConfigManager.h"
#include "../Config/Configurable.h"
#include "../Network/NetBuffer.h"
#include "../Network/NetClient.h"
#include "../Network/MessageType.h"
#include "../Utils/Util.h"
#include "../Utils/SmarterDashboard.h"
#include "../Process/AsyncProcess.h"

/*!
 * @brief Interfaces with the auto-aim sensor on the offboard processor
 * 
 * @author Tony Peng
 */
class AutoAim : public Configurable, public AsyncProcess
{
public:
	AutoAim();
	~AutoAim();
	
	void Configure();
	
	virtual INT32 Tick();
	
private:
	NetClient* m_client;
	data::ComponentData* m_componentData;
	int m_maxError;
	int m_desiredX;
	int m_desiredY;
	int m_currentX;
	int m_currentY;
};

#endif
