#ifndef COMPONENT_MANAGER_H_
#define COMPONENT_MANAGER_H_

#include <string>
#include <map>

#include "Component.h"
#include "../Utils/Util.h"

using namespace std;

/*!
 * @brief Provides an interface to manage components
 * @author Tony Peng
 */
class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();
	
	void Update();

	void CreateComponents();

	void AddComponent(Component* comp);
	void AddComponent(string id, Component* comp);
	Component* GetComponent(string id);

	void EnableComponent(string id);
	void DisableComponent(string id);

	bool IsComponentEnabled(string id);
	bool HasComponent(string id);

private:
	static bool deleteAllComponents(Component* comp);

	map<string, Component*> m_components;
};

#endif
