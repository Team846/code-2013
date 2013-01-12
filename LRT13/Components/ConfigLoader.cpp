#include "ConfigLoader.h"

ConfigLoader::ConfigLoader()
: Component("ConfigLoader")
{
	m_config = ConfigManager::Instance();
}

void ConfigLoader::onDisable()
{
	if (m_actionData->configLoaderData->load)
	{
		m_config->Load();
		m_actionData->configLoaderData->load = false;
	} else if (m_actionData->configLoaderData->save)
	{
		m_config->Save();
		m_actionData->configLoaderData->save = false;
	} else if (m_actionData->configLoaderData->apply)
	{
		m_config->ConfigureAll();
		m_actionData->configLoaderData->apply = false;
	}
}
