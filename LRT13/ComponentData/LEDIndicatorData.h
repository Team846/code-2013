#ifndef LED_INDICATOR_DATA_H_
#define LED_INDICATOR_DATA_H_

#include <WPILib.h>

namespace data
{
	namespace indicators
	{
		enum Indicator
		{
			LEFT_ARROW = 0,
			RIGHT_ARROW = 1,
			UP_ARROW = 2,
			DOWN_ARROW = 3
		};
		
		class LEDIndicatorData
		{
		public:
			LEDIndicatorData();
			
			void setColorRGB(Indicator i, INT8 red, INT8 green, INT8 blue);
			
			int getColor(Indicator i);
			int getColorR(Indicator i);
			int getColorG(Indicator i);
			int getColorB(Indicator i);
			
		private:
			UINT32 m_rgb;
		};
	};
};

#endif
