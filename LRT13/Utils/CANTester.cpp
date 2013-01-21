#include "CANTester.h"
#include "../Config/ConfigManager.h"
#include "../Utils/AsyncPrinter.h"

CANTester* CANTester::m_instance = NULL;

CANTester* CANTester::Instance()
{
	if(m_instance == NULL)
		m_instance = new CANTester();
	
	return m_instance;
}

void CANTester::Finalize()
{
	DELETE(m_instance);
}

CANTester::CANTester()
	: AsyncProcess("CANTester")
{
	m_interval = 5.0;
}

CANTester::~CANTester()
{
	
}

INT32 CANTester::Tick()
{
	bool isAlive = false;
	for(unsigned int i = 0; i < AsyncCANJaguar::jaguar_vector.size(); i++)
	{
		if(AsyncCANJaguar::jaguar_vector[i]->GetBusVoltage() > 0.1)
		{
			isAlive = true;
			break;
		}
	}
	
	if(!isAlive)
	{
		AsyncPrinter::Println("[CANTester] 2CAN test failed.");
	}
	
	Wait(m_interval);
	return 0;
}

void CANTester::Configure()
{
	ConfigManager* config = ConfigManager::Instance();
	m_interval = config->Get("CANTester", "waitInterval", 5.0);
}
