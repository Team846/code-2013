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
}

void ConfigLoader::onDisable()
{
}

void ConfigLoader::enabledPeriodic()
{
	if (m_componentData->configLoaderData->IsLoadRequested())
	{
		m_config->Load();
		m_componentData->configLoaderData->RemoveLoadRequest();
	}
	else if (m_componentData->configLoaderData->IsSaveRequested())
	{
		m_config->Save();
		m_componentData->configLoaderData->RemoveSaveRequest();
	}
	else if (m_componentData->configLoaderData->IsApplyRequested())
	{
		m_config->ConfigureAll();
		m_componentData->configLoaderData->RemoveApplyRequest();
	}	
}

void ConfigLoader::disabledPeriodic()
{
	static int e = 0;
	if (++e % 5 == 0)
		m_config->CheckForFileUpdates();
}
