#include "Climber.h"
#include "../ComponentData/ClimberData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"

using namespace data::climber;

Climber::Climber() :
	Component("Climber", DriverStationConfig::DigitalIns::CLIMBER, true),
			m_winch_worm(RobotConfig::CAN::WINCH_WORM, "WinchWorm"),
			m_configSection("climber") {
	m_pneumatics = Pneumatics::Instance();
	m_state = IDLE;
}

Climber::~Climber() {

}

void Climber::onEnable() {

}

void Climber::onDisable() {
}

void Climber::enabledPeriodic() {
	switch (m_state) {
	case IDLE:
		if (m_componentData->climberData->getDesiredClimbingStep()
				== INTENDED_ARM_UP) {
			m_state = ARM_UP_INITIAL;
		}
		break;
	case ARM_UP_INITIAL:
		m_pneumatics->setClimberArm(true);
		break;
	case WAIT:
		if (m_componentData->climberData->getDesiredClimbingStep()
				== INTENDED_CLIMBING) {
			m_state = ARM_DOWN;
			m_winch_worm.setCollectionFlags(AsyncCANJaguar::OUTCURR);
		}
		break;
	case ARM_DOWN:
		m_pneumatics->setClimberArm(false);
		m_winch_worm.SetDutyCycle(-1.0);
		if (m_winch_worm.GetOutputCurrent() > m_winch_current_threshold) {
			m_winch_worm.SetDutyCycle(m_winch_engage_duty_cycle);
			m_state = ENGAGE_PTO;
		}
		break;
	case ENGAGE_PTO:
		break;
	case WINCH_UP:
		m_timer=0;
		break;
	case ENGAGE_HOOKS:
		m_pneumatics->setHookPosition(true);
		if (++m_timer > m_timer_threshold)
			m_state=DISENGAGE_PTO;
		break;
	case DISENGAGE_PTO:
		break;
	case ARM_UP_FINAL:
		m_pneumatics->setClimberArm(true);
		m_winch_worm.SetDutyCycle(1.0);
		break;
	}
}

void Climber::disabledPeriodic() {
}

void Climber::Configure() {
	m_winch_current_threshold = m_config->Get<double> (m_configSection,
			"winchCurrentThreshold", 15.0);
	m_winch_engage_duty_cycle = m_config->Get<double> (m_configSection,
			"winchEngageDutyCycle", 0.05);
	m_timer_threshold = m_config->Get<int> (m_configSection,
			"timerThreshold", 25);
}

void Climber::Log() {

}
