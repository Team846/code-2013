#ifndef LOGGABLE_H_
#define LOGGABLE_H_

class LogManager;

/*!
 * @brief Interface for logging
 * @author Raphael Chang
 */

class Loggable
{
public:
	Loggable();
	virtual ~Loggable();
	
	/*!
	 * @brief Overload this function to log when LogManager calls LogAll()
	 */
	virtual void Log() = 0;
private:
	const LogManager* m_logger;
};

#endif
