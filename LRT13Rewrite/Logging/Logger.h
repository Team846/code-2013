#ifndef LOGGER_H_
#define LOGGER_H_

//#define USE_IOLIB
#ifdef USE_IOLIB
#include <ioLib.h>
#endif
#include <vector>
#include <typeinfo>
#include <string>

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

	~Logger();

	/*!
	 * @brief Writes the header file and allocates the block of memory to log to.
	 */
	void Initialize();
	
	/*!
	 * @brief Logs all data from one cycle into the log file.
	 */
	void Run();
	
	/*!
	 * @brief Saves a number of bytes from a variable for logging.
	 * @param field pointer to the variable
	 * @param size number of bytes to write from the pointer
	 * @param name name of the field
	 * @param source name of the Loggable object that logged this field
	 */
	template<typename T> void Log(T *field, size_t size, string name, string source)
	{
		if (!initialized)
		{
			Field f = {typeid(*field).name(), source + "/" + name, size};
			fields.push_back(f);
			dataSize += size;
		}
		else
			Write(field, size);
	}

	/*!
	 * @brief Saves a variable for logging.
	 * @param field pointer to the variable
	 * @param name name of the field
	 * @param source name of the Loggable object that logged this variable
	 */
	template<typename T> void Log(T *field, string name, string source)
	{
		Log(field, sizeof(*field), name, source);
	}
	
	/*!
	 * @brief Saves a value for logging.
	 * @param value value to log
	 * @param name name of the field
	 * @param source name of the Loggable object that logged this field
	 */
	template<typename T> void Log(T value, string name, string source)
	{
		Log(&value, sizeof(value), name, source);
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
		string name;
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
	bool initialized;
	size_t dataSize;
	char *curLoc;
	void *startLoc;
};

#endif /* LOGGER_H_ */
