#ifndef LOGGER_H_
#define LOGGER_H_

//#define USE_IOLIB
#ifdef USE_IOLIB
#include <ioLib.h>
#endif
#include <vector>
#include <typeinfo>

class Loggable;

using namespace std;

/*!
 * @brief Logs data from Loggable classes to a global log file in binary format each cycle.
 */
class Logger
{
public:
	static Logger* Instance();
	static void Finalize();

	/*!
	 * @brief Logs all data from one cycle into the log file.
	 */
	void Run();
	
	/*!
	 * @brief Saves a variable for logging.
	 * @param field pointer to the variable
	 * @param name name of the field
	 */
	template<typename T> void Logger::Log(T *field, const char* name)
	{
		Field f = {typeid(*field).name(), name, sizeof(*field)};
		fields.push_back(f);
		Write(field, sizeof(*field));
	}

	/*!
	 * @brief Saves a value for logging.
	 * @param value value to log
	 * @param name name of the field
	 */
	template<typename T> void Logger::Log(T value, const char* name)
	{
		Field f = {typeid(value).name(), name, sizeof(value)};
		fields.push_back(f);
		Write(&value, sizeof(value));
	}

	
	/*!
	 * @brief Registers a Loggable object for logging.
	 * @param loggable the Loggable object to register
	 */
	static void RegisterLoggable(Loggable* loggable);

private:
	typedef struct
	{
		const char *type;
		const char *name;
		size_t size;
	} Field;
	
	Logger();

	/*!
	 * @brief Writes data to the global log file.
	 * @param field pointer to the data
	 * @param size number of bytes to write
	 */
	void Write(void* field, size_t size);
	
	static Logger *m_instance;

	static vector<Loggable*> loggables;
	vector<Field> fields;
#ifdef USE_IOLIB
	int file;
#else
	FILE* file;
#endif
};

#endif /* LOGGER_H_ */
