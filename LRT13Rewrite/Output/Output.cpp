#include "Output.h"

vector<Output*> Output::output_vector;

Output::Output(const char *name)
{
	m_name = name;
	output_vector.push_back(this);
}

const char* Output::GetName()
{
	return m_name;
}
