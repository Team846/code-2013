#ifndef LOGGABLE_H_
#define LOGGABLE_H_

#include "Logger.h"
#include <string>

/*!
 * @brief Base class for classes that can log to the global log file.
 */
class Loggable
{
public:
	Loggable(string name);
	virtual ~Loggable();
	
	/*!
	 * @brief Logs data to the Logger.
	 */
	virtual void Log() = 0;
	
protected:
	/*!
	 * @brief Saves a variable to the Logger for logging.
	 * @param field pointer to the variable
	 * @param name name of the field
	 */
	template<typename T> void Loggable::LogToFile(T *field, string name)
	{
		m_logger->Log(field, (m_name + "/" + name).c_str());
	}

	/*!
	 * @brief Saves a value to the Logger for logging.
	 * @param value value to log
	 * @param name name of the field
	 */
	template<typename T> void Loggable::LogToFile(T value, string name)
	{
		m_logger->Log(value, (m_name + "/" + name).c_str());
	}

private:
	Logger *m_logger;
	string m_name;
};

#endif /* LOGGABLE_H_ */
