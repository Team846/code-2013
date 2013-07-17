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
			
			bool IsIndicatorChangeRequested(Indicator arrow);
			void IsIndicatorChangeRequested(Indicator arrow, bool value);
			
			void SetIndicatorColor(unsigned char red, unsigned char green, unsigned char blue, Indicator arrow);
			
			char GetColorR(Indicator arrow);
			char GetColorG(Indicator arrow);
			char GetColorB(Indicator arrow);
			UINT32 GetCombinedColor(Indicator arrow);
			
	private:
			UINT32 m_indicatorColor[4];
			bool m_changeRequested[4];
		};
	};
};

#endif
