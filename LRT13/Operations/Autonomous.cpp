#include "Autonomous.h"
#include "../ComponentData/ComponentData.h"

using namespace data;

Autonomous::Autonomous(char * taskName, INT32 priority) :
	SynchronizedProcess(taskName, priority)
{
	m_data_ptr = ComponentData::GetInstance();
	m_currentState = 0;
	
	m_autonActionCount = 1;
	m_autonActions[0] = new PauseAction(100);
}

Autonomous::~Autonomous()
{

}

INT32 Autonomous::Tick()
{
	for(int i = 0; i < m_autonActionCount; i++)
	{
		m_autonActions[i]->Run();
	}
	
	return 0;
}
