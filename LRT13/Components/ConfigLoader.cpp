#include "ConfigLoader.h"
#include "../Utils/AsyncPrinter.h"

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
	if(RobotData::GetCurrentState() != RobotData::DISABLED)
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
	else
	{
		static int e = 0;
		if (++e % 5 == 0)
			m_config->CheckForFileUpdates();
	}
}

void ConfigLoader::disabledPeriodic()
{

}
