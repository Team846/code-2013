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
			OUTER = 1,
			INNER = 0
		};

		enum Speed
		{
			HIGH = 0,
			LOW = 1
		};
		
		enum ShooterSetting
		{
			OFF = 0,
			MANUAL = 1,
			ONCE = 2,
			CONTINUOUS = 3
		};
		
		enum PneumaticsState
		{
			EXTENDED = true,
			RETRACTED = false
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
				
				void SetSpeedOffset(double offset);
				double GetSpeedOffset();
				
				void SetLauncherAngleHigh();
				void SetLauncherAngleLow();
				bool ShouldLauncherBeHigh();
				
				void increaseSpeed();
				void decreaseSpeed();
				
				bool shouldIncreaseSpeedOnce();
				bool shouldDecreaseSpeedOnce();
				
				void SetShooterSetting(ShooterSetting s);
				int GetShooterSetting();
				
				int GetFrisbeeCounter();
				void SetFrisbeeCounter(int numFrisbees);
				void IncrementFrisbeeCounter();
				void DecrementFrisbeeCounter();
				void SetEnabled(bool enabled);
				bool IsEnabled();
				
			private:
				bool m_shouldIncreaseSpeed, m_shouldDecreaseSpeed;
				double acceptableSpeedError[2];
				double speed[2];
				bool atSpeed[2];
				bool extendLauncher;
				ShooterSetting shooter_setting;
				int frisbeeCounter;
				bool m_enabled;
				double speedOffset;
		};
	}
	
	
	
}

#endif
