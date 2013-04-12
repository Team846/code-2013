#ifndef COLLECTOR_DATA_H
#define COLLECTOR_DATA_H

#include "ComponentData.h"

namespace data
{
/*!
 * @brief Namespace containing data pertaining to the collector
 * @author Raphael Chang, Tony Peng
 */
namespace collector
{
	/*!
	 * @brief Describes the state of the collector.
	 * 
	 * @author Tony Peng
	 */
	namespace CollectorState
	{
		enum Enum
		{
			UP = 0,
			DOWN = 1,
		};
	}
	
	class CollectorData
	{
	public:
		CollectorData();
		
		void RunRollers();
		void StopRollers();
		void RunRollersBackwards();
		
		int RollerDirection();
		
		bool ShouldRunRollers();
		
		void Slide(CollectorState::Enum position);
		
		void SlideDown();
		void SlideUp();
	
		CollectorState::Enum GetCollectorPosition();
		
		bool IsDown();
		bool IsUp();
	private:
		bool m_running;
		int dir;
		CollectorState::Enum m_collectorState;
	};
}
}

#endif
