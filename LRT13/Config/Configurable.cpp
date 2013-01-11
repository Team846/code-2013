#include "Configurable.h"
#include "ConfigManager.h"

Configurable::Configurable()
:m_config(ConfigManager::Instance())
{
	ConfigManager::Register(this);
}

Configurable::~Configurable()
{
	
}
