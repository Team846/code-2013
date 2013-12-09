#include "ComponentData.h"
#include "DrivetrainData.h"

map<string, ComponentData*> ComponentData::componentData_map;
vector<ComponentData*> ComponentData::data;

ComponentData::ComponentData(string name) :
	Loggable(name)
{
	componentData_map[name] = this;
}

void ComponentData::Initialize()
{
	data.push_back(new DrivetrainData());
}

void ComponentData::Finalize()
{
	for (vector<ComponentData*>::iterator it = data.begin(); it < data.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
}

ComponentData* ComponentData::GetComponentData(string name)
{
	if (componentData_map.find(name) != componentData_map.end())
		return componentData_map[name];
	return NULL;
}

void ComponentData::ResetAllCommands()
{
	for (vector<ComponentData*>::iterator it = data.begin(); it < data.end(); it++)
	{
		(*it)->ResetCommands();
	}
}
