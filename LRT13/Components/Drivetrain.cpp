#include "Drivetrain.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Sensors/DriveEncoders.h"
#include "../Utils/Utils.h"
#include "../Jaguar/ESC.h"

using namespace data;
using namespace drivetrain;

Drivetrain::Drivetrain()
: Component("Drivetrain")
 , m_driveEncoders(DriveEncoders::GetInstance())
{
	m_escs[LEFT]  = new ESC(RobotConfig::can::LEFT_DRIVE_A, RobotConfig::can::LEFT_DRIVE_B,
		m_driveEncoders.getEncoder(data::drivetrain::LEFT), "left");
	m_escs[RIGHT] = new ESC(RobotConfig::can::RIGHT_DRIVE_A, RobotConfig::can::RIGHT_DRIVE_B ,
		m_driveEncoders.getEncoder(data::drivetrain::RIGHT), "right");
}

Drivetrain::~Drivetrain()
{
	
}

void Drivetrain::UpdateActionData()
{
	//TODO update actiondata. This is prolly the place to give sems too.
}

double Drivetrain::ComputeOutput(data::drivetrain::ForwardOrTurn axis)
{
	double positionSetpoint = m_actionData->drivetrainData->getRelativePositionSetpoint(axis); // this will tell you how much further to go
	double velocitySetpoint = m_actionData->drivetrainData->getVelocitySetpoint(axis);
	double rawOutput = m_actionData->drivetrainData->getOpenLoopOutput(axis);
	switch (m_actionData->drivetrainData->getControlMode(axis))
	{
	case data::drivetrain::POSITION_CONTROL:
		m_PIDs[POSITION][axis].setInput(positionSetpoint);
		m_PIDs[POSITION][axis].setSetpoint(0.0);
		velocitySetpoint = m_PIDs[POSITION][axis].update( 1.0 / RobotConfig::LOOP_RATE);
		break;
	case data::drivetrain::VELOCITY_CONTROL:
		if (axis == data::drivetrain::FORWARD)
			m_PIDs[VELOCITY][axis].setInput(m_driveEncoders.getNormalizedForwardSpeed());
		else
			m_PIDs[VELOCITY][axis].setInput(m_driveEncoders.getNormalizedTurningSpeed());
			
		m_PIDs[VELOCITY][axis].setSetpoint(velocitySetpoint);
		
		
		rawOutput = m_PIDs[VELOCITY][axis].update(1.0 / RobotConfig::LOOP_RATE);
		break;
	case data::drivetrain::OPEN_LOOP:
		break;
	}
	return rawOutput;
}

void Drivetrain::onEnable()
{
	double fwdOutput = ComputeOutput(data::drivetrain::FORWARD); //positive means forward
	double turnOutput = ComputeOutput(data::drivetrain::TURN);   //positive means turning counter-clockwise. Matches the way driveencoders work.
	
	double leftOutput = fwdOutput - turnOutput;
	double rightOutput = fwdOutput + turnOutput;
	
	Utils::Clamp<double>(leftOutput, -1.0, 1.0);
	Utils::Clamp<double>(rightOutput, -1.0, 1.0);
	
	m_escs[LEFT]->SetDutyCycle(leftOutput);
	m_escs[LEFT]->SetDutyCycle(rightOutput);
}

void Drivetrain::onDisable()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[LEFT]->SetDutyCycle(0.0);
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
	
	pid->setParameters(p, i, d, 1.0, 0.87, feedForward);//super high decay, this makes it just like a filter. If you want it to act more like an integral you reduce the decay. This must be tuned. 
}
