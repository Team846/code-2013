#ifndef CONFIG_PORT_MAPPINGS_H_
#define CONFIG_PORT_MAPPINGS_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <WPILib.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

/*!
 * @brief Provides an interface for accessing robot configuration.
 * @author Raphael Chang
 */

class ConfigPortMappings
{
public:
	static ConfigPortMappings* Instance();
	static void Finalize();
	
	~ConfigPortMappings();
	
	void Load();
	
	/*!
	 * @brief Returns value of key at section from loaded file. A default value is required for correct type to be returned.
	 */
	static uint32_t Get(string name);
	
private:
	ConfigPortMappings();
	static ConfigPortMappings* m_instance;
	
	/*!
	 * @brief Loads the config file into memory
	 */
	void LoadConfig(string path);
	
	list<string> *fileData;
	map<string, map<string, uint32_t> > *portData;
	
	bool KeyExists(string section, string key);
	
	const static string ConfigPortMappings::CONFIG_FILE_PATH;
	const static string ConfigPortMappings::COMMENT_DELIMITERS;
	
	DISALLOW_COPY_AND_ASSIGN(ConfigPortMappings);
};

#endif
