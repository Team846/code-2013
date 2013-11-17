#ifndef CONFIG_LOADER_DATA_H
#define CONFIG_LOADER_DATA_H
#include "ComponentData.h"

namespace data
{
	/*!
	 * @brief Namespace containing data pertaining to the configuration loader.
	 * @author Raphael Chang, Tony Peng
	 */
	namespace configloader
	{
		class ConfigLoaderData
		{
		public:
			ConfigLoaderData();
			
			bool IsLoadRequested();
			bool IsSaveRequested();
			bool IsApplyRequested();
			
			void RequestLoad();
			void RequestSave();
			void RequestApply();
			
			void RemoveLoadRequest();
			void RemoveSaveRequest();
			void RemoveApplyRequest();
			
		private:
			bool m_loadRequested;
			bool m_saveRequested;
			bool m_applyRequested;
		};
	}
}

#endif
