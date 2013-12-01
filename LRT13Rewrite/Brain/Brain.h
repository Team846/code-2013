#ifndef BRAIN_H_
#define BRAIN_H_

#include <string>
#include <WPILib.h>
#include "Automation.h"
#include "InputProcessor.h"
#include <list>

using namespace std;

/*!
 * @brief Controls all automation and input processing. Coordinates and sends commands to components.
 */
class Brain
{
public:
	static Brain* Instance();
	static void Initialize();
	static void Finalize();
	
	~Brain();
	
	void Update();
	
private:
	enum BlockedAction
	{
		IGNORE,
		ABORT_SELF,
		ABORT_OTHER,
		OVERRIDE
	};
	Brain();
	
	void ProcessAutomationTasks();
	void ProcessInputs();
	
	static Brain* m_instance;
	
	vector<InputProcessor*> m_inputs;
	
	list<Automation*> m_runningTasks;
	map<Automation*, Event*> m_abortingTasks;
	map<Automation*, Event*> m_queuedTasks;
	map<Automation*, map<Automation*, BlockedAction> > m_blockedActions;
	
	DISALLOW_COPY_AND_ASSIGN(Brain);
};

#endif
