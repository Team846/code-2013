#ifndef COLLECTOR_ROLLERS_H_
#define COLLECTOR_ROLLERS_H_

#include "../Config/Configurable.h"
#include "Component.h"
#include "../ComponentData/CollectorRollersData.h"
#include "../Actuators/LRTJaguar.h"

/*!
 * @brief Provides control over the collector rollers.
 */
class CollectorRollers : public Component, public Configurable
{
public:
	CollectorRollers();
	~CollectorRollers();

	void OnEnabled(); 
	void OnDisabled();
	
	void UpdateEnabled();
	void UpdateDisabled();
	
	void Configure();
	
private:
	CollectorRollersData *m_collectorRollersData;
	LRTJaguar *m_jaguar;
	float m_speed;
};	

#endif
