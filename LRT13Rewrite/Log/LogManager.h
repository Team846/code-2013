#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include <WPILib.h>
#include <vector>
#include "Loggable.h"
#include "../Utils/Defines.h"
#include "../Process/SynchronizedProcess.h"

/*!
 * @brief Manages all instances of Loggable.
 * @author Raphael Chang, Tony Peng
 */

class LogManager : public SynchronizedProcess
{
public:
	static LogManager* Instance();
	static void Finalize();
	
	~LogManager();
	static void Register(Loggable* loggable);
	static void LogAll();
protected:
	INT32 Tick();
private:
	LogManager();
	static LogManager* m_instance;
	static vector<Loggable*> loggables;
	DISALLOW_COPY_AND_ASSIGN(LogManager);
};

#endif
