#include "ConfigLoader.h"

#warning needs implementation

ConfigLoader::ConfigLoader()
: Component("ConfigLoader", -1, false)
{
	m_config = ConfigManager::Instance();
}

ConfigLoader::~ConfigLoader()
{
	
}

void ConfigLoader::onEnable()
{
	m_isEnabled = true;
}

void ConfigLoader::onDisable()
{
	m_isEnabled = false;
}

void ConfigLoader::enabledPeriodic()
{
	if (m_componentData->configLoaderData->load)
	{
		m_config->Load();
		m_componentData->configLoaderData->load = false;
	} else if (m_componentData->configLoaderData->save)
	{
		m_config->Save();
		m_componentData->configLoaderData->save = false;
	} else if (m_componentData->configLoaderData->apply)
	{
		m_config->ConfigureAll();
		m_componentData->configLoaderData->apply = false;
	}	
}

void ConfigLoader::disabledPeriodic()
{
	static int e = 0;
	if (++e % 5 == 0)
		m_config->CheckForFileUpdates();
}
