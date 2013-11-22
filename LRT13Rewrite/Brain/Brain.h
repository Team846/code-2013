#ifndef BRAIN_H_
#define BRAIN_H_

#include <string>
#include <WPILib.h>
#include "Automation.h"
#include <vector>
#include "DebouncedJoystick.h"

using namespace std;

class Brain
{
public:
	static Brain* Instance();
	static void Initialize();
	static void Finalize();
	
	~Brain();
	
	void Update();
	
private:
	Brain();
	
	void ProcessInputs();
	
	static Brain* m_instance;
	
	vector<Automation*> m_automation;
	
	DebouncedJoystick *m_driver_stick;
	DebouncedJoystick *m_operator_stick;
	DebouncedJoystick *m_driver_wheel;
	
	DISALLOW_COPY_AND_ASSIGN(Brain);
};

#endif
