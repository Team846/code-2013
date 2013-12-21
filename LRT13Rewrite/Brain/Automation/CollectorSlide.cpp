#include "CollectorSlide.h"
#include "Angle.h"
#include "CollectorPosition.h"
#include "Pause.h"

CollectorSlide::CollectorSlide(bool down) :
	Sequential("CollectorSlide"),
	Configurable("collector")
{
	m_down = down;
	if (m_down)
	{
		AddAutomation(new Angle(false));
		AddAutomation(new CollectorPosition(true));
		AddAutomation(new Pause(m_extendCycles));
		AddAutomation(new Angle(true));
	}
	else
	{
		AddAutomation(new Angle(false));
		AddAutomation(new Pause(m_retractCycles));
		AddAutomation(new CollectorPosition(false));
	}
}

CollectorSlide::~CollectorSlide()
{

}

void CollectorSlide::AllocateResources()
{
	AllocateResource(ANGLE);
	AllocateResource(COLLECTOR_SLIDE);
}

void CollectorSlide::Configure()
{
	m_extendCycles = m_config->Get<int>(m_configSection, "cyclesBeforeExtend", 50);
	m_retractCycles = m_config->Get<int>(m_configSection, "cyclesBeforeRetract", 20);
	ClearSequence();
	if (m_down)
	{
		AddAutomation(new Angle(false));
		AddAutomation(new CollectorPosition(true));
		AddAutomation(new Pause(m_extendCycles));
		AddAutomation(new Angle(true));
	}
	else
	{
		AddAutomation(new Angle(false));
		AddAutomation(new Pause(m_retractCycles));
		AddAutomation(new CollectorPosition(false));
	}
}

