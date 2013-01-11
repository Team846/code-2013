#include "LogManager.h"

LogManager* LogManager::m_instance = NULL;

LogManager* LogManager::Instance()
{
	if (m_instance == NULL)
		m_instance = new LogManager();
	return m_instance;
}

void LogManager::Register(Loggable* loggable)
{
	loggables.push_back(loggable);
}

void LogManager::LogAll()
{
	for (vector<Loggable*>::iterator it = loggables.begin(); it < loggables.end(); it++)
	{
		(*it)->Log();
	}
}
