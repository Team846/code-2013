#ifndef COLLECTOR_SLIDE_H_
#define COLLECTOR_SLIDE_H_

#include <string>
#include "Sequential.h"
#include "../../Actuators/Pneumatics.h"

/*!
 * @brief Sequential routine that slides the collector up or down while changing angle.
 */
class CollectorSlide : public Sequential, public Configurable
{
public:
	CollectorSlide(bool down);
	~CollectorSlide();
	
	void Configure();
	
protected:
	void AllocateResources();
	
private:
	int m_extendCycles;
	int m_retractCycles;
	bool m_down;
};

#endif
