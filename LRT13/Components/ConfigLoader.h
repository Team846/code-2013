#ifndef CONFIG_LOADER_H_
#define CONFIG_LOADER_H_

#include "Component.h"
#include "../ComponentData/ConfigLoaderData.h"
#include "../Config/ConfigManager.h"

class ConfigLoader : public Component
{
public:
	ConfigLoader();
	~ConfigLoader();
	
	virtual void UpdateActionData(); 
	virtual void onEnable(); 
	virtual void onDisable();
private:
	ConfigManager* m_config;
};

#endif
