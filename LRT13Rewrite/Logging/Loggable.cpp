#include "Loggable.h"

Loggable::Loggable(string name) :
	m_name(name)
{
	m_logger = Logger::Instance();
	Logger::RegisterLoggable(this);
}

Loggable::~Loggable()
{
	
}
