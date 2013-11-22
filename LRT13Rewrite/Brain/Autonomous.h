#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_

#include "Automation.h"
//#include "../ComponentData/DrivetrainData.h"
//#include "../ComponentData/AutoAimData.h"
//#include "../ComponentData/ConfigLoaderData.h"
#include "Routines/Routine.h"

#include <queue>
#include <fstream>
#include <string>

//namespace data
//{
//class ComponentData;
//}

/*!
 * @brief Performs autonomous routine and contains autonomous functions.
 * @author Raphael Chang, Manoj Vasishta
 */
class Autonomous : public Automation
{
public:
	Autonomous();
	~Autonomous();
	
	void Start(Event *trigger);
	void Run();
	void Abort(Event *trigger);
	
private:
    void LoadRoutine(std::string path);
    
	double m_autonomousStartTime;
	
//	data::ComponentData *m_componentData;
	
    queue<Routine*> routines;
    bool beginNext;
};

#endif
