#ifndef AUTOAIM_H_
#define AUTOAIM_H_

#include "../ComponentData/ComponentData.h"
#include "../ComponentData/AutoAimData.h"
#include "../Config/ConfigManager.h"
#include "../Config/Configurable.h"
#include "../Network/NetBuffer.h"
#include "../Network/NetServer.h"
#include "../Network/MessageType.h"
#include "../Utils/Util.h"
#include "../Utils/SmarterDashboard.h"
#include "../Process/SynchronizedProcess.h"

/*!
 * @brief Interfaces with the auto-aim sensor on the offboard processor
 * 
 * @author Raphael Chang, Tony Peng
 */
class AutoAim : public Configurable, public SynchronizedProcess
{
public:
	AutoAim();
	~AutoAim();
	
	void Configure();
	
	virtual INT32 Tick();
	
private:
	NetServer* m_server;
	data::ComponentData* m_componentData;
	int m_maxError;
	int m_desiredX;
	int m_desiredY;
	int m_currentX;
	int m_currentY;
};

#endif
