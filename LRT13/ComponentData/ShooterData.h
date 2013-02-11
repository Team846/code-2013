#ifndef SHOOTER_DATA_H
#define SHOOTER_DATA_H

#include "ComponentData.h"

namespace data
{
	/*!
	 * @brief Contains methods and data to access shooter data.
	 * @author Tony Peng
	 */
	namespace shooter
	{
	
		enum shooterState
		{
			DISABLED, RUNNING
		};
		
		enum shooterConfiguration
		{
			SLOW, LESSSLOW
		};
	
		class ShooterData
		{
			public:
				ShooterData();
				data::shooter::shooterConfiguration desiredTarget;
				data::shooter::shooterState state;
				double speed;
			private:
				bool atSpeed;
				
				
		};
	}
	
	
	
}

#endif
