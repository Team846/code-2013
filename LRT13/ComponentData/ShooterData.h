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
				
				void SetAcceptableSpeedError(double error, int roller);
				double GetAcceptableSpeedError(int roller);
				
				void SetLauncherAngleHigh();
				void SetLauncherAngleLow();
				bool ShouldLauncherBeHigh();
				
				void ShootOnce();
				void ShootContinuous();
				
				int GetNumFrisbeesInStorage();
				void SetNumFrisbeesInStorage(int numFrisbees);
				void IncrementFrisbeeCounter();
				void DecrementFrisbeeCounter();
				
			private:
				double acceptableSpeedError[2];
				double speed[2];
				bool atSpeed[2];
				bool extendLauncher;
				
				int frisbeeCounter;
		};
	}
	
	
	
}

#endif
