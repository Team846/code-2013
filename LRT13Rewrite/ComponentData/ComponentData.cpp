#include "ComponentData.h"
#include "DrivetrainData.h"

map<string, ComponentData*> ComponentData::componentData_map;
vector<ComponentData*> ComponentData::data;

ComponentData::ComponentData(string name) :
	Loggable(name)
{
	AddComponentData(this, name);
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

void ComponentData::AddComponentData(ComponentData *data, string name)
{
	componentData_map[name] = data;
}

ComponentData* ComponentData::GetComponentData(string name)
{
	return componentData_map[name];
}
