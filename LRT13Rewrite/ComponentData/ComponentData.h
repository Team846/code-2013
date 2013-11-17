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
		class WinchPawlData;
	}
	namespace indicators
	{
		class LEDIndicatorData;
	}
	namespace autoaim
	{
		class AutoAimData;
	}
	namespace routinerecorder
	{
		class RoutineRecorderData;
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
	
		~ComponentData();
		
		drivetrain::DrivetrainData* const drivetrainData;
		shooter::ShooterData* const shooterData;
		collector::CollectorData* const collectorData;
		climber::ClimberData* const climberData;
		climber::WinchPawlData* const winchPawlData;
		configloader::ConfigLoaderData* const configLoaderData;
		indicators::LEDIndicatorData* const ledIndicatorData;
		autoaim::AutoAimData* const autoAimData;
		routinerecorder::RoutineRecorderData* const routineRecorderData;
	
	protected:
		ComponentData();
		
		DISALLOW_COPY_AND_ASSIGN(ComponentData);
	
	private:
		static ComponentData* m_instance;
	};
}

#endif
