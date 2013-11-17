#ifndef AUTONOMOUS_ROUTINES_H_
#define AUTONOMOUS_ROUTINES_H_

#include "../Process/SynchronizedProcess.h"
#include "../ComponentData/RobotData.h"
#include "../ComponentData/DrivetrainData.h"
#include "../ComponentData/AutoAimData.h"
#include "../ComponentData/ConfigLoaderData.h"
#include "Routines/Routine.h"
#include "DebouncedJoystick.h"

#include <queue>
#include <fstream>
#include <string>

namespace data
{
class ComponentData;
}
class AutoActions;

/*!
 * @brief Performs autonomous routine and contains autonomous functions. It has two parts, the Tick function which is meant to run every cycle to operate semi-automated processes
 * @author Raphael Chang, Tony Peng, Brian Axelrod, Manoj Vasishta
 */
class AutonomousRoutines
{
public:
	AutonomousRoutines();
	~AutonomousRoutines();
	
	void TeleopTick(); //called every cycle to manage semi-autonomous functions
	
	void Update();
	
private:
    void LoadRoutine(std::string path);
    
	void ServiceAutoAimBackBoard();
		
	double m_autonomousStartTime;
	
	AutoActions* m_autoActions;

	data::ComponentData *m_componentData;
	
    queue<Routine*> routines;
    bool beginNext;
    
    DebouncedJoystick *m_joystick;
    
//	typedef struct Cycle
//	{
//		double forward;
//		double turn;
//		bool collecting;
//		bool shooting;
//		bool angleHigh;
//	};
};

#endif
