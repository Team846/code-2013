#ifndef CONFIGURABLE_H_
#define CONFIGURABLE_H_

class ConfigRuntime;

/*!
 * @brief Interface for reading from configuration file through ConfigManager
 * @author Raphael Chang
 */

class Configurable
{
public:
	Configurable();
	virtual ~Configurable();
	
	/*!
	 * @brief Overload this function to apply new configuration when configuration is loaded
	 */
	virtual void Configure() = 0;
	
protected:
	ConfigRuntime * const m_config;
};

#endif
