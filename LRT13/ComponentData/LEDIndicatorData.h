#ifndef LED_INDICATOR_DATA_H_
#define LED_INDICATOR_DATA_H_

#include <WPILib.h>

namespace data
{
	namespace indicators
	{
		enum Indicator
		{
			RIGHT_ARROW = 0,
			UP_ARROW = 1,
			LEFT_ARROW = 2,
			DOWN_ARROW = 3
		};
		
		class LEDIndicatorData
		{
		public:
			LEDIndicatorData();
			
			void setColorRGB(INT8 red, INT8 green, INT8 blue, Indicator arrow);
			
			INT8 getColorR(Indicator arrow);
			INT8 getColorG(Indicator arrow);
			INT8 getColorB(Indicator arrow);
			
		private:
			INT8 r[4];
			INT8 g[4];
			INT8 b[4];
		};
	};
};

#endif
