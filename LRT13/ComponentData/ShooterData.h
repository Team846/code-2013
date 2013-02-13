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
				void SetDesiredSpeed(double desiredSpeed, Roller roller);
				double GetDesiredSpeed(Roller roller);
				void SetAtSpeed(bool value, Roller roller);
				bool IsAtSpeed(Roller roller);
				double acceptableSpeedError[2];
				void SetAcceptableSpeedError(double error, int roller);
				double GetAcceptableSpeedError(int roller);
			private:
				double speed[2];
				bool atSpeed[2];
		};
	}
	
	
	
}

#endif
