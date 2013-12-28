#ifndef BRAIN_H_
#define BRAIN_H_

#include <string>
#include <WPILib.h>
#include "Automation/Automation.h"
#include "InputProcessor/InputProcessor.h"
#include <list>
#include "../Logging/Loggable.h"

using namespace std;

/*!
 * @brief Controls all automation and input processing. Coordinates and sends commands to components.
 */
class Brain : public Loggable
{
public:
	static Brain* Instance();
	static void Initialize();
	static void Finalize();
	
	~Brain();
	
	void Update();
	
	void Log();
	
private:
	Brain();
	
	void ProcessAutomationTasks();
	void ProcessInputs();
	
	static Brain* m_instance;
	
	vector<InputProcessor*> m_inputs;
	vector<Automation*> m_automation;
	
	list<Automation*> m_runningTasks;
	map<Automation*, Event*> m_waitingTasks;
	
	DISALLOW_COPY_AND_ASSIGN(Brain);
};

#endif
