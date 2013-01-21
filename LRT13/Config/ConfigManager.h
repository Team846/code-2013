#ifndef CONFIG_MANAGER_H_
#define CONFIG_MANAGER_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <WPILib.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include "Configurable.h"

using namespace std;

/*!
 * @brief Singleton for loading, reading, and saving configuration files
 * @author Raphael Chang
 */

struct Config
{
	string value;
	list<string>::iterator position;
};
typedef map<string, map<string, Config> > config;

class ConfigManager
{
public:
	static ConfigManager* Instance();
	static void Finalize();
	
	~ConfigManager();
	
	void Load();
	void Save();
	/*!
	 * @brief Returns value of key at section from loaded file. A default value is required for correct type to be returned.
	 */
	template<typename T> T Get(string section, string key, T defaultValue);
	/*!
	 * @brief Sets value of key at section in memory. Does not save to config file until Save() is called.
	 */
	template<typename T> void Set(string section, string key, T value);
	/*!
	 * @brief Registers a configurable into configurable list
	 */
	static void Register(Configurable* configurable);
	/*!
	 * @brief Calls Configure() on all registered configurables
	 */
	static void ConfigureAll();
	/*!
	 * @brief Checks if file has been changed
	 */
	void CheckForFileUpdates();
	
private:
	ConfigManager();
	static ConfigManager* m_instance;
	static vector<Configurable*> configurables;
	
	/*!
	 * @brief Loads the config file into memory
	 */
	void LoadConfig(string path);	
	/*!
	 * @brief Saves config data from memory into config file
	 */
	void SaveConfig(string path);
	list<string> *fileData;
	config* configData;
	map<string, list<string>::iterator> *sectionMap;
	time_t lastReadTime;
	bool KeyExists(string section, string key);
	string Trim(string str);
	
	const static string ConfigManager::CONFIG_FILE_PATH;
	const static string ConfigManager::COMMENT_DELIMITERS;
	
	DISALLOW_COPY_AND_ASSIGN(ConfigManager);
};

#endif
