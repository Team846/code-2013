#include "ConfigPortMappings.h"
#include "RobotConfig.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"
#include <fstream>
#include <sstream>

ConfigPortMappings* ConfigPortMappings::m_instance = NULL;
const string ConfigPortMappings::CONFIG_FILE_PATH = RobotConfig::PORT_MAPPINGS_FILE_PATH;
const string ConfigPortMappings::COMMENT_DELIMITERS = "#;";

ConfigPortMappings* ConfigPortMappings::Instance()
{
	if (m_instance == NULL)
		m_instance = new ConfigPortMappings();
	return m_instance;
}

void ConfigPortMappings::Finalize()
{
	DELETE(m_instance);
}

ConfigPortMappings::ConfigPortMappings()
{
	fileData = new list<string>();
	portData = new map<string, map<string, uint32_t> >();
}

ConfigPortMappings::~ConfigPortMappings()
{
	delete fileData;
	fileData = NULL;
	delete portData;
	portData = NULL;
}

void ConfigPortMappings::Load()
{
	LoadConfig(CONFIG_FILE_PATH);
	printf("ConfigPortMappings: Done loading %s\n", CONFIG_FILE_PATH.c_str());
}

uint32_t ConfigPortMappings::Get(string name)
{
	stringstream sstream(name);
	string section, key;
	getline(sstream, section, '/');
	getline(sstream, key);
	transform(section.begin(), section.end(), section.begin(), tolower);
	transform(key.begin(), key.end(), key.begin(), toupper);
	if (Instance()->KeyExists(section, key))
	{
		return (*Instance()->portData)[section][key];
	}
	printf("[ERROR] Port mapping not found for %s\n", name.c_str());
	return 0;
}

bool ConfigPortMappings::KeyExists(string section, string key)
{
	return portData->find(section) != portData->end()
			&& (*portData)[section].find(key) != (*portData)[section].end();
}

void ConfigPortMappings::LoadConfig(string path)
{
	ifstream fin(path.c_str());

	if (!fin.is_open())
	{
		printf("[ERROR] ConfigPortMappings: Could not open %s for reading\n",
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
				ConfigPortMappings::COMMENT_DELIMITERS.c_str()); // String length up to first comment
		if (length == string::npos) // If no comments on this line
			length = it->length();

		string line = Util::Trim(it->substr(0, length)); // Trim whitespace from non-comment part of this line
		if (line.length() == 0) // If this line contains no data
			continue;

		if (line[0] == '[') // If new section
		{
			currentSection = line.substr(1, line.find_last_of("]") - 1); // Set current section name
			transform(currentSection.begin(), currentSection.end(), currentSection.begin(), tolower);
			continue;
		}

		stringstream sstream(line);
		string key;
		uint32_t value;
		getline(sstream, key, '='); // Get key up to =
		sstream >> value; // Get assigned value after =
		transform(key.begin(), key.end(), key.begin(), toupper);
		(*portData)[currentSection][key] = value; // Set value for key of current section
	}
}
