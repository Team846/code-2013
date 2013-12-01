#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include <vector>

using namespace std;

/*!
 * @brief Base class for all actuators.
 */
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
