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

template<typename T> void Loggable::LogToFile(T *field, string name)
{
	m_logger->Log(field, (m_name + "/" + name).c_str());
}

template<typename T> void Loggable::LogToFile(T value, string name)
{
	m_logger->Log(value, (m_name + "/" + name).c_str());
}
