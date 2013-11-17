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
	Output();
	virtual const char* GetName() = 0;
	virtual void Update() = 0;
	
	static vector<Output*> output_vector;
};

#endif
