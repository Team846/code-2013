#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <vector>

/*!
 * @brief Interface for all output resources.
 * 
 * @author Raphael Chang
 */

using namespace std;

class Output
{
public:
	Output(const char *name);
	virtual const char* GetName();
	virtual void Update() = 0;
	
	static vector<Output*> output_vector;
	
private:
	const char *m_name;
};

#endif
