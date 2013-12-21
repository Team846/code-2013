#ifndef GAME_MODE_CHANGE_EVENT_H_
#define GAME_MODE_CHANGE_EVENT_H_

#include "Event.h"
#include "../../RobotState.h"

/*!
 * @brief Event that fires when the game mode changes to a particular mode.
 */
class GameModeChangeEvent : public Event
{
public:
	GameModeChangeEvent(RobotState::Mode toMode);
	GameModeChangeEvent(RobotState::Mode fromMode, RobotState::Mode toMode);
	virtual ~GameModeChangeEvent();
	
	virtual bool Fired();
	virtual bool CheckCondition();
	
private:
	RobotState::Mode m_toMode;
	RobotState::Mode m_fromMode;
	bool m_from;
};

#endif
