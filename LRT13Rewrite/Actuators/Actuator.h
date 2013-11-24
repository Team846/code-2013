#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <vector>
#include <Error.h>

/*!
 * @brief Interface for all actuators.
 * 
 * @author Raphael Chang
 */

using namespace std;

class Actuator
{
public:
	explicit Actuator(const char *name);
	virtual const char* GetName();
	virtual void Update() = 0;
	
	static void UpdateAll();
	
private:
	static vector<Actuator*> actuator_vector;
	const char *m_name;
};

#endif
