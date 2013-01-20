#include "ConfigLoader.h"

#warning needs implementation

ConfigLoader::ConfigLoader()
: Component("ConfigLoader")
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

void ConfigLoader::Update()
{
	if(RobotData::GetCurrentState() == RobotData::DISABLED)
	{
		static int e = 0;
		if (++e % 5 == 0)
			m_config->CheckForFileUpdates();
	}
	else
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
}
