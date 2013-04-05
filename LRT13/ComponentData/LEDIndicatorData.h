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
			
			void setColorRGB(INT8 red, INT8 green, INT8 blue);
			
			int getColor();
			int getColorR();
			int getColorG();
			int getColorB();
			
		private:
			UINT32 m_rgb;
		};
	};
};

#endif
