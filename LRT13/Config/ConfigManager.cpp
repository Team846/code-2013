#include "ConfigManager.h"
#include "RobotConfig.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Utils.h"
#include <fstream>
#include <sstream>

ConfigManager* ConfigManager::m_instance = NULL;
const string ConfigManager::CONFIG_FILE_PATH = RobotConfig::CONFIG_FILE_PATH;
const string ConfigManager::COMMENT_DELIMITERS = "#;";

ConfigManager* ConfigManager::Instance()
{
	if (m_instance == NULL)
		m_instance = new ConfigManager();
	return m_instance;
}

ConfigManager::ConfigManager()
{
	AsyncPrinter::Printf("Loaded ConfigManager\n");
}

ConfigManager::~ConfigManager()
{
	if (fileData != NULL)
		delete fileData;

	if (configData != NULL)
		delete configData;

	if (sectionMap != NULL)
		delete sectionMap;
}

void ConfigManager::Load()
{
	LoadConfig(CONFIG_FILE_PATH);
	AsyncPrinter::Printf("Done loading %s\n", CONFIG_FILE_PATH.c_str());
	ConfigureAll();
}

void ConfigManager::Save()
{
	SaveConfig(CONFIG_FILE_PATH);
}

template bool ConfigManager::Get<bool>(string section, string key, bool defaultValue);
template double ConfigManager::Get<double>(string section, string key, double defaultValue);
template string ConfigManager::Get<string>(string section, string key, string defaultValue);
template int ConfigManager::Get<int>(string section, string key, int defaultValue);

template<typename T> T ConfigManager::Get(string section, string key, T defaultValue)
{
	if (KeyExists(section, key))
	{
		stringstream sstream((*configData)[section][key].value);
		T value;
		sstream >> value;
		return value;
	} else
	{
		return defaultValue;
	}
}

template void ConfigManager::Set<bool>(string section, string key, bool val);
template void ConfigManager::Set<int>(string section, string key, int val);
template void ConfigManager::Set<double>(string section, string key, double val);
template void ConfigManager::Set<string>(string section, string key, string val);

template<typename T> void ConfigManager::Set(string section, string key, T value)
{
	string newValue = Util::ToString<T>(value);
	// Edit file data to save to file when Save() is called
	if (KeyExists(section, key))
	{
		Config nameValuePair = (*configData)[section][key];
		list<string>::iterator valueLocation = nameValuePair.position;
		string oldValue = nameValuePair.value;

		// Location of the first occurrence of the old value after the equals sign
		unsigned int indexOfStartOfOldValue = valueLocation->find(oldValue, valueLocation->find(key));
		unsigned int indexOfEquals = valueLocation->find(oldValue, valueLocation->find("="));
		string newString = valueLocation->substr(0, indexOfStartOfOldValue);
		newString += key + "=";
		newString += newValue;
		newString += valueLocation->substr(min(indexOfEquals + oldValue.size(), valueLocation->size()));
		*valueLocation = newString;
	} else
	{
		if (configData->find(section) == configData->end()) // If section does not exist
		{
			fileData->push_back(string("[") + section + "]");
			list<string>::iterator sectionLocation = fileData->end();
			sectionLocation--;
			(*sectionMap)[section] = sectionLocation;
		}
		list<string>::iterator sectionLocation = (*sectionMap)[section];
		sectionLocation++;
		string str = key + "=" + newValue;
		fileData->insert(sectionLocation, str);
	}
	(*configData)[section][key].value = newValue; // Update current config data
}

bool ConfigManager::KeyExists(string section, string key)
{
	return configData->find(section) != configData->end()
			&& (*configData)[section].find(key)
					!= (*configData)[section].end();
}

void ConfigManager::Register(Configurable* configurable)
{
	configures.push_back(configurable);
}

void ConfigManager::ConfigureAll()
{
	AsyncPrinter::Printf("Applying configuration to all configurables\n");
	for(vector<Configurable*>::iterator it = configures.begin(); it < configures.end(); it++)
	{
		(*it)->Configure();
	}
}

void ConfigManager::LoadConfig(string path)
{
	ifstream fin(path.c_str());
	
	if (!fin.is_open())
	{
		AsyncPrinter::Printf("ConfigManager could not open %s for reading\n", path.c_str());
		return;
	}
	
	// Clear previous data
	if (!fileData->empty())
	{
		fileData->clear();
	}
	if (!sectionMap->empty())
	{
		sectionMap->clear();
	}
	
	// Read lines into list
	while (!fin.eof())
	{
		string line;
		getline(fin, line);
		fileData->push_back(line);
	}
	fin.close();

	string currentSection = "";
	for (list<string>::iterator it = fileData->begin(); it != fileData->end(); it++)
	{
		unsigned int length = it->find_first_of(ConfigManager::COMMENT_DELIMITERS.c_str()); // String length up to first comment
		if (length == string::npos) // If no comments on this line
			length = it->length();
		
		string line = Trim(it->substr(0, length)); // Trim whitespace from non-comment part of this line
		if (line.length() == 0) // If this line contains no data
			continue;
		
		if (line[0] == '[') // If new section
		{
			currentSection = line.substr(1, line.find_last_of("]") - 1); // Set current section name
			(*sectionMap)[currentSection] = it; // Set position of current section
			continue;
		}
		
		stringstream sstream(line);
		string key, value;
		getline(sstream, key, '='); // Get key up to =
		getline(sstream, value); // Get assigned value after =
		Config currentConfig;
		currentConfig.value = value;
		currentConfig.position = it;
		(*configData)[currentSection][key] = currentConfig; // Set value for key of current section
	}
}

void ConfigManager::SaveConfig(string path)
{
	ofstream fout(path.c_str());
	if (!fout.is_open())
	{
		AsyncPrinter::Printf("ConfigManager could not open %s for writing\n", path.c_str());
		return;
	}

	for (list<string>::iterator it = fileData->begin(); it != fileData->end(); it++)
	{
		fout << *it << '\n';
	}

	fout.close();
}

string ConfigManager::Trim(string str)
{
	int startIndex = -1, endIndex = str.size() - 1;

	// Find first non-whitespace index
	for (unsigned int i = 0; i < str.size(); i++)
	{
		if (!isspace(str[i]))
		{
			startIndex = i;
			break;
		}
	}

	if (startIndex == -1)
		return ""; // Blank line

	// Find index of end of non-whitespace
	for (int i = str.size() - 1; i >= 0; i--)
	{
		if (!isspace(str[i]))
		{
			endIndex = i;
			break;
		}
	}

	return str.substr(startIndex, endIndex - startIndex + 1);
}
