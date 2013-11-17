#ifndef ROUTINE_RECORDER_DATA_H
#define ROUTINE_RECORDER_DATA_H
#include "ComponentData.h"

namespace data
{
	/*!
	 * @brief Namespace containing data pertaining to the configuration loader.
	 * @author Raphael Chang, Tony Peng
	 */
	namespace routinerecorder
	{
		class RoutineRecorderData
		{
		public:
			RoutineRecorderData();
			
			bool ShouldRecord();
			void ShouldRecord(bool record);
			
		private:
			bool m_recordRequested;
		};
	}
}

#endif
