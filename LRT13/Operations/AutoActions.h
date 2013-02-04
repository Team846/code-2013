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
		AUTO_AIM,
		AUTO_CLIMB,
		DISC_TRACK
	};
	AutoActions();
	bool AutoAim();
	bool AutoClimb();
	bool DiscTrack();
	void Reset();
private:
	ComponentData* m_componentData;
	static actions m_currentAction;
	static unsigned int m_counter;
};

#endif
