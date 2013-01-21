#include "Collector.h"
#include "../ComponentData/CollectorData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

Collector::Collector()
: Component("Collector", DriverStationConfig::DigitalIO::COLLECTOR, true)
{
	m_jaguar = new AsyncCANJaguar(RobotConfig::can::COLLECTOR, "Collector");
}
