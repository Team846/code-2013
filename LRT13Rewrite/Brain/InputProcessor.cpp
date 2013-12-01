#include "InputProcessor.h"

InputProcessor::InputProcessor()
{
	
}

InputProcessor::~InputProcessor()
{
	
}

bool InputProcessor::CheckResources()
{
	for (vector<ComponentData*>::iterator it = resources.begin(); it < resources.end(); it++)
	{
		if (Automation::GetAllocation(*it) != NULL)
			return false;
	}
	return true;
}

void InputProcessor::RegisterResource(ComponentData *resource)
{
	resources.push_back(resource);
}
