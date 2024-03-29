#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <WPILib.h>

#include <list>
#include <string>

#include "../ComponentData/ComponentData.h"

class ComponentManager;

/*!
 * @brief Generic abstract class for components
 * @author Tony Peng, Raphael Chang
 */
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
	
	/*!
	 * @brief Constructs a component with a name
	 */
	Component(std::string name, int dio, bool requiresEnabledState);
	
	/*!
	 * @brief Frees the resources allocated by the component.
	 */
	virtual ~Component();
	
	/*!
	 * @brief Allows the component to update itself when enabled.
	 */
	virtual void enabledPeriodic() = 0;
	
	/*!
	 * @brief Allows the component to execute code when disabled.
	 */
	virtual void disabledPeriodic() = 0;
	
	/*!
	 * @brief Event handler for when the component is enabled.
	 */
	virtual void onEnable() = 0;
	
	/*!
	 * @brief Event handler for when the component is disabled.
	 */
	virtual void onDisable() = 0;
	
	/*!
	 * @brief Enables the component.
	 */
	void Enable();
	
	/*!
	 * @brief Disables the component.
	 */
	void Disable();
	
	/*!
	 * @brief Returns whether or not the component is enabled.
	 */
	bool IsEnabled();
	
	/*!
	 * @brief Returns whether or not the component requires to be in an enabled state.
	 */
	bool EnableRequired();
	
	/*!
	 * @brief Gets the digital i/o channel for the component
	 */
	int GetDIO();
	
	/*!
	 * @brief Gets the component's name.
	 */
	std::string GetName();
	
protected:
	data::ComponentData * const m_componentData;
private:
	std::string m_name;
	bool m_enabled;
	
	bool m_requiresEnabled;
	
	int m_DIO;
};

#endif //COMPONENT_H_
