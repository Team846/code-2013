#include "Actuator.h"
#include <ErrorBase.h>
#include "../Utils/AsyncPrinter.h"

vector<Actuator*> Actuator::actuator_vector;

Actuator::Actuator(const char *name)
{
	m_name = name;
	actuator_vector.push_back(this);
}

const char* Actuator::GetName()
{
	return m_name;
}

void Actuator::UpdateAll()
{
	for (vector<Actuator*>::iterator it = Actuator::actuator_vector.begin(); it < Actuator::actuator_vector.end(); it++)
	{
		if (dynamic_cast<ErrorBase*>(*it))
		{
			if (dynamic_cast<ErrorBase*>(*it)->StatusIsFatal())
			{
				AsyncPrinter::Printf("[ERROR] Fatal error in %s: %s\n", (*it)->GetName(), dynamic_cast<ErrorBase*>(*it)->GetError().GetMessage());
			}
		}
		(*it)->Update();
	}
}

