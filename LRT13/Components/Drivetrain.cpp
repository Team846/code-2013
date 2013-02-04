#include "Drivetrain.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../Sensors/DriveEncoders.h"
#include "../Utils/Util.h"
#include "../Utils/AsyncPrinter.h"
#include "../Jaguar/ESC.h"

using namespace data;
using namespace drivetrain;

Drivetrain::Drivetrain()
: Component("Drivetrain", DriverStationConfig::DigitalIns::DRIVETRAIN, true)
 , m_driveEncoders(DriveEncoders::GetInstance())
{
	m_escs[LEFT]  = new ESC(RobotConfig::CAN::LEFT_DRIVE_A, RobotConfig::CAN::LEFT_DRIVE_B,
		m_driveEncoders->getEncoder(data::drivetrain::LEFT), "left");
	m_escs[RIGHT] = new ESC(RobotConfig::CAN::RIGHT_DRIVE_A, RobotConfig::CAN::RIGHT_DRIVE_B ,
		m_driveEncoders->getEncoder(data::drivetrain::RIGHT), "right");
	lastPositionSetpoint[FORWARD] = m_componentData->drivetrainData->getRelativePositionSetpoint(FORWARD);
	lastPositionSetpoint[TURN] = m_componentData->drivetrainData->getRelativePositionSetpoint(TURN);
	lastTravelledDistance = m_driveEncoders->getRobotDist();
}

Drivetrain::~Drivetrain()
{
	
}

double Drivetrain::ComputeOutput(ForwardOrTurn axis)
{
	// TODO: Add turn position control
	double positionSetpoint = m_componentData->drivetrainData->getRelativePositionSetpoint(axis); // this will tell you how much further to go
	if (axis == FORWARD)
	{
		if (lastPositionSetpoint[axis] != positionSetpoint)
		{
			lastTravelledDistance = m_driveEncoders->getRobotDist();
			lastPositionSetpoint[axis] = positionSetpoint;
		}
	}
	double velocitySetpoint = m_componentData->drivetrainData->getVelocitySetpoint(axis);
	double rawOutput = m_componentData->drivetrainData->getOpenLoopOutput(axis);
	switch (m_componentData->drivetrainData->getControlMode(axis))
	{
	case POSITION_CONTROL:
		if (axis == FORWARD)
		{
			m_PIDs[POSITION][axis].setInput(m_driveEncoders->getRobotDist());
			m_PIDs[POSITION][axis].setSetpoint(lastTravelledDistance + positionSetpoint);
			velocitySetpoint = Util::Clamp<double>(m_PIDs[POSITION][axis].update(1.0 / RobotConfig::LOOP_RATE),
					m_componentData->drivetrainData->getPositionControlMaxSpeed(axis),
					-m_componentData->drivetrainData->getPositionControlMaxSpeed(axis));
		}
		// Fall through the switch
	case VELOCITY_CONTROL:
		if (axis == data::drivetrain::FORWARD)
			m_PIDs[VELOCITY][axis].setInput(m_driveEncoders->getNormalizedForwardSpeed());
		else
			m_PIDs[VELOCITY][axis].setInput(m_driveEncoders->getNormalizedTurningSpeed());
			
		m_PIDs[VELOCITY][axis].setSetpoint(velocitySetpoint);
		
		rawOutput = m_PIDs[VELOCITY][axis].update(1.0 / RobotConfig::LOOP_RATE);
		break;
	case OPEN_LOOP:
		break;
	}
	return rawOutput;
}

void Drivetrain::onEnable()
{
}

void Drivetrain::onDisable()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[RIGHT]->SetDutyCycle(0.0);
}

void Drivetrain::enabledPeriodic()
{
	double fwdOutput = ComputeOutput(FORWARD); //positive means forward
	double turnOutput = ComputeOutput(TURN);   //positive means turning counter-clockwise. Matches the way DriveEncoders work.
	
	double leftOutput = fwdOutput - turnOutput;
	double rightOutput = fwdOutput + turnOutput;
	
	Util::Clamp<double>(leftOutput, -1.0, 1.0);
	Util::Clamp<double>(rightOutput, -1.0, 1.0);

	m_escs[LEFT]->SetDutyCycle(leftOutput);
	m_escs[RIGHT]->SetDutyCycle(rightOutput);
}

void Drivetrain::disabledPeriodic()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[RIGHT]->SetDutyCycle(0.0);
}

void Drivetrain::updateData()
{
	// TODO: Add error threshold
	if (lastTravelledDistance + m_componentData->drivetrainData->getRelativePositionSetpoint(FORWARD) == m_driveEncoders->getRobotDist())
	{
		semGive(m_componentData->drivetrainData->positionOperationSemaphore(FORWARD, 0));
	}
}

void Drivetrain::Configure()
{
	ConfigurePIDObject(&m_PIDs[VELOCITY][TURN], "velocity_turn", true);
	ConfigurePIDObject(&m_PIDs[VELOCITY][FORWARD], "velocity_fwd", true);
//	m_PIDs[VELOCITY][TURN].setIIREnabled(true);
//	m_PIDs[VELOCITY][FWD].setIIREnabled(true);

	ConfigurePIDObject(&m_PIDs[POSITION][TURN], "position_turn", false);
	ConfigurePIDObject(&m_PIDs[POSITION][FORWARD], "position_fwd", false);
}

void Drivetrain::Log()
{
	
}

void Drivetrain::ConfigurePIDObject(PID *pid, std::string objName, bool feedForward)
{
	double p = m_config->Get<double>(Component::GetName(), objName + "_P", 1.0);
	double i = m_config->Get<double>(Component::GetName(), objName + "_I", 0.0);
	double d = m_config->Get<double>(Component::GetName(), objName + "_D", 0.0);
	
	pid->setParameters(p, i, d, 1.0, 0.87, feedForward); // Super high decay, this makes it just like a filter. If you want it to act more like an integral you reduce the decay. This must be tuned. 
}
