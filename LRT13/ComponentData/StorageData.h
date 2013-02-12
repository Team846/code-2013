#ifndef STORAGE_DATA_H
#define STORAGE_DATA_H

#include "ComponentData.h"

namespace data
{
	/*!
	 * @brief Contains methods and data to access frisbee storage component.
	 * @author Tony Peng, Raphael Chang
	 */

	namespace storage
	{
		class StorageData
		{
		public:
			StorageData();
			~StorageData();
			
			void IncrementFrisbeeCount();
			void DecrementFrisbeeCount();
			
			int GetFrisbeeCount();
		private:
			int m_frisbeeCount;
		};
	}
	
	
	
}

#endif
