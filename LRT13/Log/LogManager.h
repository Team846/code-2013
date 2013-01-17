#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include <WPILib.h>
#include <vector>
#include "Loggable.h"

/*!
 * @brief Singleton for handling logging
 * @author Raphael Chang
 */

class LogManager
{
public:
	static LogManager* Instance();
	~LogManager();
	static void Register(Loggable* loggable);
	static void LogAll();
private:
	LogManager();
	static LogManager* m_instance;
	static vector<Loggable*> loggables;
	DISALLOW_COPY_AND_ASSIGN(LogManager);
};

#endif
