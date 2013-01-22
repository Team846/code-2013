#include "Component.h"
#include "../Config/DriverStationConfig.h"
#include "../Config/RobotConfig.h"
#include "../Utils/AsyncPrinter.h"

/*!
 * @brief Test component for the Component system
 * @author Tony Peng
 */
class ComponentSystemUnitTest : public Component
{
public:
	/*!
	 * @brief Constructs a component with a name
	 */
	ComponentSystemUnitTest();
	
	/*!
	 * @brief Frees the resources allocated by the component.
	 */
	~ComponentSystemUnitTest();

	/*!
	 * @brief Event handler for when the component is enabled.
	 */
	virtual void onEnable();
	
	/*!
	 * @brief Event handler for when the component is disabled.
	 */
	virtual void onDisable();
	
	/*!
	 * @brief Allows the component to update itself when enabled.
	 */
	virtual void whenEnabled();
	
	/*!
	 * @brief Allows the component to execute code when disabled.
	 */
	virtual void whenDisabled();
};