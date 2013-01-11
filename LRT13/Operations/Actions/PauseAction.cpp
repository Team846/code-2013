#include "PauseAction.h"

PauseAction::PauseAction(int millis)
{
	m_millisSleep = millis;
}

PauseAction::~PauseAction() { }

int PauseAction::Run()
{
	sleep(m_millisSleep);
	
	return 0;
}
