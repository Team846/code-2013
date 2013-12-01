#include "Logger.h"
#include "Loggable.h"
#include "../Config/RobotConfig.h"
#include "../RobotState.h"

Logger *Logger::m_instance = NULL;
vector<Loggable*> Logger::loggables;

Logger* Logger::Instance()
{
	if (m_instance == NULL)
		m_instance = new Logger();
	return m_instance;
}

void Logger::Finalize()
{
	delete m_instance;
	m_instance = NULL;
}

Logger::Logger()
{
}

void Logger::Run()
{
	if (RobotState::Instance().GameMode() == RobotState::DISABLED)
	{
		if (RobotState::Instance().LastGameMode() != RobotState::DISABLED)
#ifdef USE_IOLIB
			close(file);
#else
			fclose(file);
#endif
	}
	else
	{
		if (RobotState::Instance().LastGameMode() == RobotState::DISABLED)
#ifdef USE_IOLIB
			file = open(RobotConfig::LOG_FILE_PATH.c_str(), O_CREAT | O_WRONLY, 0777);
#else
			file = fopen(RobotConfig::LOG_FILE_PATH.c_str(), "wb");
#endif
	
		for (vector<Loggable*>::iterator it = loggables.begin(); it < loggables.end(); it++)
		{
			(*it)->Log();
		}
#ifndef USE_IOLIB
		fflush(file);
#endif

		if (RobotState::Instance().LastGameMode() == RobotState::DISABLED)
		{
			FILE* header = fopen((RobotConfig::LOG_FILE_PATH + ".header").c_str(), "w");
			fprintf(header, "%d\n", fields.size());
			for (vector<Field>::iterator it = fields.begin(); it < fields.end(); it++)
			{
				fprintf(header, "%s %s %d\n", it->type, it->name, it->size);
			}
		}
		fields.clear();
	}
}

template<typename T> void Logger::Log(T *field, const char* name)
{
	Field f = {typeid(*field).name(), name, sizeof(*field)};
	fields.push_back(f);
	Write(field, sizeof(*field));
}

template<typename T> void Logger::Log(T value, const char* name)
{
	Field f = {typeid(value).name(), name, sizeof(value)};
	fields.push_back(f);
	Write(&value, sizeof(value));
}

void Logger::Write(void* field, size_t size)
{
	if (RobotState::Instance().GameMode() != RobotState::DISABLED)
#ifdef USE_IOLIB
		write(file, (char*)field, size);
#else
		fwrite(field, size, 1, file);
#endif
}

void Logger::RegisterLoggable(Loggable *loggable)
{
	loggables.push_back(loggable);
}
