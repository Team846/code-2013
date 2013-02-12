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
	namespace configLoader
	{
		class ConfigLoaderData;
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
		configLoader::ConfigLoaderData* const configLoaderData;
	
	protected:
		ComponentData();
		
		
		DISALLOW_COPY_AND_ASSIGN(ComponentData);
	
	private:
		static ComponentData* m_instance;
	};
}

#endif
