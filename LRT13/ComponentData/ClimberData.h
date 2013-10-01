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
		NOTHING = -1,
		BEGIN = 0,
		LINE_UP = 1,
		HANG = 2,
		ARMS_DOWN = 3,
		CLIMB_PREPARE = 4,
		CLIMB = 5,
		DEBUG_MODE = 100,
		
//		IDLE = 1,
//		ARM_UP_INITIAL = 2,
//		WAIT = 3,
//		ARM_DOWN = 4,
//		ENGAGE_PTO = 5,
//		WINCH_UP = 6,
//		ENGAGE_HOOKS = 7,
//		DISENGAGE_PTO = 8,
//		ARM_UP_FINAL = 9,
//		DUMB_ENGAGE_PTO = 10,
//		UNLOCK_PAWL = 11,
//		PREPARE_CLIMBING_POSITION = 12,
//		DO_NOTHING= 13
	};

typedef enum ClimbState
{
	GROUND = 0,
	TEN_POINT = 1,
	TWENTY_POINT = 2,
	THIRTY_POINT = 3
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
		ClimberData();
		humanState getDesiredClimbingStep();
		void setDesiredClimbingStep(humanState newState);
		bool shouldContinueClimbing();
		bool shouldForceContinueClimbing();
		void setShouldContinueClimbing(bool shouldContinue);
		void setShouldForceContinueClimbing(bool shouldContinue);
		
		bool shouldPotentiallyAbort();
		bool setShouldPotentiallyAbort(bool shouldAbort);

		//Debug functions
		bool shouldDebug();
		void enableDebug();
		void disableDebug();

		void changeArmState();
		
		bool shouldChangeAngleState();
		void changeAngleState();
		
		bool shouldWinchPawlGoUp();
		bool shouldWinchPawlGoDown();
		void winchPawlUp();
		void winchPawlDown();
		void winchPawlInactive();
		
		double winchPawlCurrent();
		void setWinchPawlCurrent(double value);
		
		bool shouldPTOChangeDisengage();
		bool shouldPTOChangeEngage();
		bool EngagePTO();
		bool DisengagePTO();
		
		void changeHooks();
		
		bool shouldChangeHooks();
		
		void extendArm();
		void retractArm();
		
		bool shouldArmsUp();
		bool shouldArmsChange();
		
		void setDesiredState(state target);
		state getDesiredState();
		void setCurrentState(state newState);
		state getCurrentState();
		void setWaitingState(state newState);
		state getWaitingState();
		void setShouldHooksChange(bool hooksDown);
		bool shouldHooksChange();
	private:
		humanState m_desiredClimbingStep;
		bool m_shouldContinueClimbing, m_shouldForceContinueClimbing;
		
		bool m_shouldPotentiallyAbort;
		
		bool m_shouldWinchPawlGoUp;
		bool m_shouldWinchPawlGoDown;
		
		bool m_shouldPTOEngage;
		bool m_shouldPTODisEngage;
		
		bool m_shoulddebug;
		
		bool m_armState;
		bool m_shouldChangeArm;
		
		bool m_shouldHooksChange;
		
		double m_winchPawlCurrent;
		
		state m_desiredState;
		state m_currentState;
		state m_waitingState;
	};
}
}

#endif
