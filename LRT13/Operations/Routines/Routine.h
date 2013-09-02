#ifndef ROUTINE_H_
#define ROUTINE_H_

#include "../../ComponentData/ComponentData.h"

/*!
 * @brief Provides an interface for robot routines.
 * @author Tony Peng, Raphael Chang
 */
class Routine
{
public:
	Routine()
	{
		m_isComplete = false;
		m_componentData = data::ComponentData::GetInstance();
	}
	virtual ~Routine() { };
	
	virtual void Run() = 0;
	
	virtual bool Completed() = 0;
	
protected:
	bool m_isComplete;
    data::ComponentData* m_componentData;
};

#endif
