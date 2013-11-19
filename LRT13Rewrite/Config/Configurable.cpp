#include "Configurable.h"
#include "ConfigRuntime.h"

Configurable::Configurable()
:m_config(ConfigRuntime::Instance())
{
	ConfigRuntime::Register(this);
}

Configurable::~Configurable()
{
	
}
