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
	
	enum routines
	{
		FRONT,
		BACK
	};
	
	AutoActions();
	
	void EnableAutoAimBackboard();
	void EnableAutoAimPyramid();
	void EnableFeederStationTrack();
	void Reset();
	
	actions GetCurrentAutonomousAction();
	
	void SetRoutine(routines routine);
	routines GetRoutine();
	
	void setNumFrisbeesToPickUp(int numFrisbeesToPickUp);
	int getNumFrisbeesToPickUp();
	
private:
	ComponentData* m_componentData;
	routines m_currentAutoRoutine;
	actions m_currentAction;
	unsigned int m_counter;
	int m_numFrisbeesToPickUp;
};

#endif
