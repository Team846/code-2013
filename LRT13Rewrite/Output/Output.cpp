#include "Output.h"

vector<Output*> Output::output_vector;

Output::Output()
{
	output_vector.push_back(this);
}
