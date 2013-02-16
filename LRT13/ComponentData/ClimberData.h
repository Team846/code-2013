#ifndef CLIMBER_DATA_H
#define CLIMBER_DATA_H

#include "ComponentData.h"

namespace data
{
/*!
 * @brief Namespace containing data pertaining to the climber
 * @author Eva Lomazov, Christina Lin, Michelle Chang, Brian Axelrod, Julia Huang
 */
namespace climber
{
typedef enum state
	{
		IDLE = 1,
		ARM_UP_INITIAL = 2,
		WAIT = 3,
		ARM_DOWN = 4,
		ENGAGE_PTO = 5,
		WINCH_UP = 6,
		ENGAGE_HOOKS = 7,
		DISENGAGE_PTO = 8,
		ARM_UP_FINAL = 9
	};

typedef enum humanState
{
	INTENDED_IDLE = 1,
	INTENDED_ARM_UP = 2,
	INTENDED_CLIMBING = 3
};
	class ClimberData
	{
		//Define desired state (what part of climb im working on)
		//Keep track of current state (what part of climb im working on: done, in progress; starting)
	public:
		humanState getDesiredClimbingStep();
		void setDesiredClimbingStep(humanState newState);
	
	
	};
}
}

#endif
