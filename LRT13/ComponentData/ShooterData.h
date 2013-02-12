#ifndef SHOOTER_DATA_H
#define SHOOTER_DATA_H

#include "ComponentData.h"

namespace data
{
	/*!
	 * @brief Contains methods and data to access shooter data.
	 * @author Tony Peng, Varun Parthasarathy, Raphael Chang
	 */
	namespace shooter
	{
		enum Roller
		{
			FRONT = 0,
			BACK = 1
		};
		
		class ShooterData
		{
			public:
				ShooterData();
				void setDesiredSpeed(double desiredSpeed, Roller roller);
				double getDesiredSpeed(Roller roller);
				void setAtSpeed(bool value, Roller roller);
				bool isAtSpeed(Roller roller);
			private:
				double speed[2];
				bool atSpeed[2];
		};
	}
	
	
	
}

#endif
