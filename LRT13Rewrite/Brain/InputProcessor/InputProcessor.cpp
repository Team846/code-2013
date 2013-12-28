#include "InputProcessor.h"

InputProcessor::InputProcessor()
{
	
}

InputProcessor::~InputProcessor()
{
	
}

bool InputProcessor::CheckResources()
{
	for (vector<ControlResource>::iterator it = resources.begin(); it < resources.end(); it++)
	{
		if (Automation::GetAllocation(*it))
			return false;
	}
	return true;
}

void InputProcessor::RegisterResource(ControlResource resource)
{
	resources.push_back(resource);
}
