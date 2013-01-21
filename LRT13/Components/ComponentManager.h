#ifndef COMPONENT_MANAGER_H_
#define COMPONENT_MANAGER_H_

#include <string>
#include <map>

#include "Component.h"
#include "../Utils/Util.h"

using namespace std;

typedef struct
{
	Component* component;
	bool RequiresEnabledState;
	int DIO;
} FullComponent;

class ComponentManager
{
public:
	~ComponentManager();

	static ComponentManager* ComponentManager::Instance();

	void Update();

	void CreateComponents();

	void AddComponent(string id, Component* comp, int dio, bool requiresEnabledState);
	Component* GetComponent(string id);

	void EnableComponent(string id);
	void DisableComponent(string id);

	bool IsComponentEnabled(string id);

private:
	ComponentManager();
	static bool deleteAllComponents(Component* comp);

	map<string, FullComponent> m_components;
	
	DriverStation* m_ds;

	static ComponentManager* m_instance;DISALLOW_COPY_AND_ASSIGN(ComponentManager);
};

#endif
