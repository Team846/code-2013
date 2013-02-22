#ifndef AUTO_ACTIONS_H_
#define AUTO_ACTIONS_H_

namespace data
{
class ComponentData;
}

using namespace data;

/*!
 * @brief Contains functions for automatic movement
 * @author Raphael Chang
 */
class AutoActions
{
public:
	enum actions
	{
		AUTO_AIM_BACKBOARD,
		AUTO_AIM_PYRAMID,
		FEEDER_STATION_APPROACH,
		NO_ADV_AUTOMATION
	};
	
	AutoActions();
	
	void EnableAutoAimBackboard();
	void EnableAutoAimPyramid();
	void EnableFeederStationTrack();
	void Reset();
	
	actions GetCurrentAutonomousAction();
	
private:
	ComponentData* m_componentData;
	static actions m_currentAction;
	static unsigned int m_counter;
};

#endif
