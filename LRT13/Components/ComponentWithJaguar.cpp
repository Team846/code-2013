#include "ComponentWithJaguar.h"

ComponentWithJaguar::ComponentWithJaguar(string name, int dio, bool requiresEnabled, UINT8 channel, string jag_name)
	: Component(name, dio, requiresEnabled)
{
		m_jaguar = new AsyncCANJaguar(channel, jag_name.c_str());
}

ComponentWithJaguar::~ComponentWithJaguar()
{
	DELETE(m_jaguar);
}
