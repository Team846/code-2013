#ifndef PTO_DATA_H
#define PTO_DATA_H

#include "ComponentData.h"

namespace data
{
/*!
 * @brief Namespace containing data pertaining to the PTO
 * @author Raphael Chang
 */
namespace pto
{
	class PTOData
	{
	public:
		PTOData();
		void Engage();
		void Disengage();
		bool IsEngaged();
	private:
		bool m_engaged;
	};
}
}

#endif
