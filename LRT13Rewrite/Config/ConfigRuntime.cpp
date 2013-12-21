#include "ConfigRuntime.h"
#include "RobotConfig.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"
#include <fstream>
#include <sstream>

vector<Configurable*> ConfigRuntime::configurables; // note: static vectors must be declared in the CPP file -TP
ConfigRuntime* ConfigRuntime::m_instance = NULL;
const string ConfigRuntime::CONFIG_FILE_PATH = RobotConfig::CONFIG_FILE_PATH;
const string ConfigRuntime::COMMENT_DELIMITERS = "#;";

ConfigRuntime* ConfigRuntime::Instance()
{
	if (m_instance == NULL)
		m_instance = new ConfigRuntime();
	return m_instance;
}

void ConfigRuntime::Finalize()
{
	DELETE(m_instance);
}

ConfigRuntime::ConfigRuntime()
: lastReadTime(0)
{
	fileData = NULL;
	configData = NULL;
	sectionMap = NULL;
	Load();
}

ConfigRuntime::~ConfigRuntime()
{
	if (fileData != NULL)
		DELETE(fileData);

	if (configData != NULL)
		DELETE(fileData);

	if (sectionMap != NULL)
		DELETE(sectionMap);
}

void ConfigRuntime::Load()
{
	LoadConfig(CONFIG_FILE_PATH);
	AsyncPrinter::Printf("ConfigRuntime: Done loading %s\n", CONFIG_FILE_PATH.c_str());
	ConfigureAll();
}

void ConfigRuntime::Save()
{
	SaveConfig(CONFIG_FILE_PATH);
}

template bool ConfigRuntime::Get<bool>(string section, string key,
		bool defaultValue);
template float ConfigRuntime::Get<float>(string section, string key,
		float defaultValue);
template double ConfigRuntime::Get<double>(string section, string key,
		double defaultValue);
template string ConfigRuntime::Get<string>(string section, string key,
		string defaultValue);
template int ConfigRuntime::Get<int>(string section, string key,
		int defaultValue);

template<typename T> T ConfigRuntime::Get(string section, string key,
		T defaultValue)
{
	if (KeyExists(section, key))
	{
		stringstream sstream((*configData)[section][key].value);
		T value;
		sstream >> value;
		return value;
	}
	else
	{
		Set<T>(section, key, defaultValue);
		return defaultValue;
	}
}

template void ConfigRuntime::Set<bool>(string section, string key, bool val);
template void ConfigRuntime::Set<float>(string section, string key, float val);
template void ConfigRuntime::Set<int>(string section, string key, int val);
template void
		ConfigRuntime::Set<double>(string section, string key, double val);
template void
		ConfigRuntime::Set<string>(string section, string key, string val);

template<typename T> void ConfigRuntime::Set(string section, string key,
		T value)
{
	string newValue = Util::ToString<T>(value);
	// Edit file data to save to file when Save() is called
	if (KeyExists(section, key))
	{
		Config nameValuePair = (*configData)[section][key];
		list<string>::iterator valueLocation = nameValuePair.position;
		string oldValue = nameValuePair.value;
		/*
		 // Location of the first occurrence of the old value after the equals sign
		 unsigned int indexOfStartOfOldValue = valueLocation->find(oldValue,
		 valueLocation->find(key));
		 unsigned int indexOfEquals = valueLocation->find(oldValue,
		 valueLocation->find("="));
		 string newString = valueLocation->substr(0, indexOfStartOfOldValue);
		 newString += key + "=";
		 newString += newValue;
		 newString += valueLocation->substr(min(indexOfEquals + oldValue.size(),
		 valueLocation->size()));*/
		string newString = key + "=" + newValue;
		*valueLocation = newString;
	}
	else
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

void ConfigRuntime::CheckForFileUpdates()
{
	struct stat statistics;
	stat(CONFIG_FILE_PATH.c_str(), &statistics);

	// Reload when the file has been modified
	if (lastReadTime != statistics.st_mtime)
	{
		Load();

		// Load() sometimes saves the configuration, so get the new modified time.
		stat(CONFIG_FILE_PATH.c_str(), &statistics);
		lastReadTime = statistics.st_mtime;
	}
}

bool ConfigRuntime::KeyExists(string section, string key)
{
	return configData->find(section) != configData->end()
			&& (*configData)[section].find(key) != (*configData)[section].end();
}

void ConfigRuntime::Register(Configurable* configurable)
{
	configurables.push_back(configurable);
}

void ConfigRuntime::ConfigureAll()
{
	AsyncPrinter::Printf("Applying configuration to all configurables\n");
	for (vector<Configurable*>::iterator it = configurables.begin(); it
			!= configurables.end(); it++)
	{
		(*it)->Configure();
	}
}

void ConfigRuntime::LoadConfig(string path)
{
	// Clear previous data
	if (fileData != NULL)
	{
		DELETE(fileData);
	}
	fileData = new list<string>();
	if (configData != NULL)
	{
		DELETE(configData);
	}
	configData = new config();
	if (sectionMap != NULL)
	{
		DELETE(sectionMap);
	}
	sectionMap = new map<string, list<string>::iterator>();

	ifstream fin(path.c_str());

	if (!fin.is_open())
	{
		AsyncPrinter::Printf("ConfigManager: Could not open %s for reading\n",
				path.c_str());
		return;
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
		unsigned int length = it->find_first_of(
				ConfigRuntime::COMMENT_DELIMITERS.c_str()); // String length up to first comment
		if (length == string::npos) // If no comments on this line
			length = it->length();

		string line = Util::Trim(it->substr(0, length)); // Trim whitespace from non-comment part of this line
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

void ConfigRuntime::SaveConfig(string path)
{
	ofstream fout(path.c_str());
	if (!fout.is_open())
	{
		AsyncPrinter::Printf("ConfigManager: could not open %s for writing\n",
				path.c_str());
	}

	for (list<string>::iterator it = fileData->begin(); it != fileData->end(); it++)
	{
		fout << *it << '\n';
	}
	
	fout.close();
	
	AsyncPrinter::Printf("Done saving %s\n", path.c_str());
}

