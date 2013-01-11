#include "Loggable.h"
#include "LogManager.h"

Loggable::Loggable()
:m_logger(LogManager::Instance())
{
	LogManager::Register(this);
}

Loggable::~Loggable()
{
	
}
