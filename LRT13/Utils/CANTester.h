#ifndef CAN_TESTER_H
#define CAN_TESTER_H

#include <WPILib.h>

#include "../Process/AsyncProcess.h"
#include "../Jaguar/AsyncCANJaguar.h"
#include "../Config/Configurable.h"

using namespace std;

/*!
 * @brief Loops through all Jaguars to determine if 2CAN is alive
 * @author Tony Peng, Raphael Chang
 */

class CANTester : public AsyncProcess, public Configurable
{
public:
	static CANTester* Instance();
	static void Finalize();
	
	CANTester();
	~CANTester();
	void Configure();
protected:
	INT32 Tick();
private:
	static CANTester* m_instance;
	double m_interval;
};

#endif
