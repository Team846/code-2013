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
		AUTO_AIM_BACKBOARD = 1,
		AUTO_AIM_PYRAMID = 2,
		FEEDER_STATION_APPROACH = 3,
		NO_ADV_AUTOMATION = 4
	};
	
	enum routines
	{
		FRONT,
		BACK
	};
	
	static AutoActions* Instance();
	static void Finalize();
	
	AutoActions();
	~AutoActions();
	
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
	static AutoActions* m_instance;
	ComponentData* m_componentData;
	routines m_currentAutoRoutine;
	actions m_currentAction;
	unsigned int m_counter;
	int m_numFrisbeesToPickUp;
};

#endif
