#include "LogManager.h"
#include "Loggable.h"

Loggable::Loggable()
:m_logger(LogManager::Instance())
{
	LogManager::Register(this);
}

Loggable::~Loggable()
{
	
}
