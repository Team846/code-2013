#ifndef WINCH_PAWL_DATA_H_
#define WINCH_PAWL_DATA_H_

#include "ComponentData.h"

namespace data
{
	namespace climber
	{
		class WinchPawlData
		{
		public:
			WinchPawlData();
			
			void setDutyCyle(float f);
			float getDutyCycle();

			void updateMotorCurrent(double d); // this should ONLY be used by the winch pawl component
			double getMotorCurrent();
			
			bool isWinchPawlTimedOut();
			void setWinchPawlTimedOut(bool b); // this should ONLY be used by the winch pawl component
			
		private:
			float m_dutyCycle;
			double m_current;
			
			bool m_isWinchPawlTimedOut;
		};
	}
}
#endif
