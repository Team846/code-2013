#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "WPILib.h"
#include <list>
#include <string>
#include "../ComponentData/ComponentData.h"

class Component
{
public:
	struct ComponentWithData
	{
		bool RequiresEnabledState;
		const static int NO_DS_DISABLE_DIO = -1;
		int DS_DIOToDisableComponent;
		Component *component;
	};
	
	Component(std::string name);
	virtual ~Component();

	virtual void UpdateActionData() = 0;
	virtual void OnEnable() = 0;
	virtual void OnDisable() = 0;
	
	std::string GetName();
	
	/*!
	 * @brief the Factory method that constructs all the components. This makes it so that the main loop does not have to know about the individual components.
	 * @return a list structs with components and information about the components. The information about the component includes whether the output method should be called if the robot is disabled as well as which digital io on the driverstation should disable this component. 
	 */
	static list<ComponentWithData> CreateComponents();
protected:
	data::ComponentData * const m_actionData;
private:
	std::string m_name;
};

#endif //COMPONENT_H_
