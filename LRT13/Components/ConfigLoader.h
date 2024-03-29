#ifndef CONFIG_LOADER_H_
#define CONFIG_LOADER_H_

#include "../ComponentData/RobotData.h"

#include "Component.h"
#include "../ComponentData/ConfigLoaderData.h"
#include "../Config/ConfigManager.h"

using namespace data;

class ConfigLoader : public Component
{
public:
	ConfigLoader();
	~ConfigLoader();

	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void enabledPeriodic();
	virtual void disabledPeriodic();
	
private:
	ConfigManager* m_config;
};

#endif
