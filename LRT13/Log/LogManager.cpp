#include "LogManager.h"

LogManager* LogManager::m_instance = NULL;
vector<Loggable*> LogManager::loggables; // note: static vectors must be declared in the CPP file -TP

LogManager* LogManager::Instance()
{
	if (m_instance == NULL)
		m_instance = new LogManager();
	return m_instance;
}

LogManager::LogManager()
{ }

LogManager::~LogManager()
{ }

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
