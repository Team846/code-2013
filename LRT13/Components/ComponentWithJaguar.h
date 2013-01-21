#ifndef COMPONENT_WITH_JAGUAR_H_
#define COMPONENT_WITH_JAGUAR_H_

#include "WPILib.h"
#include <list>
#include <string>

#include "Component.h"

#include "../Jaguar/AsyncCANJaguar.h"

// TO-DO: should refactor/rename me

/*!
 * @brief Describes a component that has a jaguar speed controller.
 * @author Tony Peng
 */
class ComponentWithJaguar : public Component
{
public:
	ComponentWithJaguar(string name, int dio, bool requiresEnabled, UINT8 channel, string name);
	virtual ~ComponentWithJaguar();

protected:
	AsyncCANJaguar* m_jaguar;
};

#endif
