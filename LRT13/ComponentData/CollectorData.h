#ifndef COLLECTOR_DATA_H
#define COLLECTOR_DATA_H

#include "ComponentData.h"

namespace data
{
/*!
 * @brief Namespace containing data pertaining to the collector
 * @author Raphael Chang
 */
namespace collector
{
	class CollectorData
	{
	public:
		CollectorData();
		void run();
	private:
		bool m_running;
	};
}
}

#endif
