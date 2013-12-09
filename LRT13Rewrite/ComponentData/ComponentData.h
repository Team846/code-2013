#ifndef COMPONENT_DATA_H_
#define COMPONENT_DATA_H_

#include <WPILib.h>
#include <string>
#include <map>
#include <vector>
#include "../Logging/Loggable.h"

using namespace std;

/*!
 * @brief Base class for objects containing commands and states of components.
 */
class ComponentData : public Loggable
{
public:
	ComponentData(string name);
	
	static void Initialize();
	static void Finalize();

	static void ResetAllCommands();
	
protected:
	virtual void ResetCommands() = 0;
	
	static ComponentData* GetComponentData(string name);
	
private:
	static map<string, ComponentData*> componentData_map;
	static vector<ComponentData*> data;
};

#endif
