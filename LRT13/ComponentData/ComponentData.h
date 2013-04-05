#ifndef COMPONENT_DATA_H
#define COMPONENT_DATA_H

#include <WPILib.h>
#include "../Utils/Defines.h"

namespace data
{
	namespace drivetrain
	{
		class DrivetrainData;
	}
	namespace shooter
	{
		class ShooterData;
	}
	namespace configloader
	{
		class ConfigLoaderData;
	}
	namespace shooter
	{
		class ShooterData;
	}
	namespace collector
	{
		class CollectorData;
	}
	namespace climber
	{
		class ClimberData;
	}
	
	namespace indicators
	{
		class LEDIndicatorData;
	}
	
	/*!
	 * @brief Provides a central hub for data access.
	 * @author Tony Peng, Brian Axelrod, Raphael Chang
	 */
	class ComponentData
	{
	public:
		static ComponentData* GetInstance();
		static void Finalize();
	
		
		drivetrain::DrivetrainData* const drivetrainData;
		shooter::ShooterData* const shooterData;
		collector::CollectorData* const collectorData;
		climber::ClimberData* const climberData;
		configloader::ConfigLoaderData* const configLoaderData;
		indicators::LEDIndicatorData* const ledIndicatorData;
	
	protected:
		ComponentData();
		
		
		DISALLOW_COPY_AND_ASSIGN(ComponentData);
	
	private:
		static ComponentData* m_instance;
	};
}

#endif
