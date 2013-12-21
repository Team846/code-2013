#include "InputProcessor.h"

InputProcessor::InputProcessor()
{
	
}

InputProcessor::~InputProcessor()
{
	
}

bool InputProcessor::CheckResources()
{
	for (vector<Automation::Resource>::iterator it = resources.begin(); it < resources.end(); it++)
	{
		if (Automation::GetAllocation(*it) != NULL)
			return false;
	}
	return true;
}

void InputProcessor::RegisterResource(Automation::Resource resource)
{
	resources.push_back(resource);
}
