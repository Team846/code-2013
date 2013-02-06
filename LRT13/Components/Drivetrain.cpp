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
}

Drivetrain::~Drivetrain()
{
	
}

double Drivetrain::ComputeOutput(ForwardOrTurn axis)
{
	double positionSetpoint = m_componentData->drivetrainData->getRelativePositionSetpoint(axis); // this will tell you how much further to go
	
	// TODO: Add turn position control
	double velocitySetpoint = m_componentData->drivetrainData->getVelocitySetpoint(axis);
	double rawOutput = m_componentData->drivetrainData->getOpenLoopOutput(axis);
	switch (m_componentData->drivetrainData->getControlMode(axis))
	{
	case POSITION_CONTROL:
		if (axis == FORWARD)
		{
			m_PIDs[POSITION][axis].setInput(m_driveEncoders->getRobotDist());
			m_PIDs[POSITION][axis].setSetpoint(m_componentData->drivetrainData->getPositionControlStartingPosition(axis) + positionSetpoint);
			velocitySetpoint = Util::Clamp<double>(m_PIDs[POSITION][axis].update(1.0 / RobotConfig::LOOP_RATE),
					-m_componentData->drivetrainData->getPositionControlMaxSpeed(axis),
					m_componentData->drivetrainData->getPositionControlMaxSpeed(axis));
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
	//TODO: turn
	m_componentData->drivetrainData->updatePositions(m_driveEncoders->getRobotDist(), m_driveEncoders->getTurnAngle());
	m_componentData->drivetrainData->updateVelocities(m_driveEncoders->getNormalizedForwardSpeed(), m_driveEncoders->getNormalizedTurningSpeed());
	// Check if position has been achieved and release the operation semaphore
	if (fabs(m_componentData->drivetrainData->getPositionControlStartingPosition(FORWARD) + m_componentData->drivetrainData->getRelativePositionSetpoint(FORWARD) - m_driveEncoders->getRobotDist()) <= m_errorThreshold)
	{
		semGive(m_componentData->drivetrainData->positionOperationSemaphore(FORWARD, 0));
	}
	
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

void Drivetrain::Configure()
{
	ConfigurePIDObject(&m_PIDs[VELOCITY][TURN], "velocity_turn", true);
	ConfigurePIDObject(&m_PIDs[VELOCITY][FORWARD], "velocity_fwd", true);
//	m_PIDs[VELOCITY][TURN].setIIREnabled(true);
//	m_PIDs[VELOCITY][FWD].setIIREnabled(true);

	ConfigurePIDObject(&m_PIDs[POSITION][TURN], "position_turn", false);
	ConfigurePIDObject(&m_PIDs[POSITION][FORWARD], "position_fwd", false);
	
	m_errorThreshold = m_config->Get<double>(Component::GetName(), "position_fwd_error", 1.0);
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
