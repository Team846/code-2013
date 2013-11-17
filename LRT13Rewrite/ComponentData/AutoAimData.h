#ifndef AUTOAIM_DATA_H_
#define AUTOAIM_DATA_H_

#include "ComponentData.h"

namespace data
{
	namespace autoaim
	{
		class AutoAimData
		{
		public:
			AutoAimData();
			
			void setErrorThreshold(int threshold);
			void setDesiredX(int x);
			void setDesiredY(int y);
			void setCurrentX(int x);
			void setCurrentY(int y);
			void setStrength(double new_strength);
			int getErrorThreshold();
			int getDesiredX();
			int getDesiredY();
			int getCurrentX();
			int getCurrentY();
			double getStrength();
		private:
			int errorThreshold;
			int desiredX;
			int desiredY;
			int currentX;
			int currentY;
			double strength;
		};
	};
};

#endif
