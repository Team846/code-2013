#ifndef INPUT_PROCESSOR_H_
#define INPUT_PROCESSOR_H_

#include "../../DriverStation/LRTDriverStation.h"
#include "../Automation/Automation.h"
#include "../ControlResource.h"

/*!
 * @brief Base for joystick input processors.
 */
class InputProcessor
{
public:
	InputProcessor();
	virtual ~InputProcessor();
	
	/*!
	 * @brief Processes the joysticks.
	 */
	virtual void Update() = 0;
	/*!
	 * @brief Checks that all used resources are not allocated.
	 * @return whether all used resources are not allocated
	 */
	bool CheckResources();
	
protected:
	/*!
	 * @brief Adds a resource to be used.
	 * @param resource the resource to add
	 */
	void RegisterResource(ControlResource resource);
	
private:
	vector<ControlResource> resources;
};

#endif
