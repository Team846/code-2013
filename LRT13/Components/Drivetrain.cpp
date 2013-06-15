#include "Drivetrain.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Sensors/DriveEncoders.h"
#include "../Utils/Util.h"
#include "../SpeedController/ESC.h"
#include "../Config/DriverStationConfig.h"

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

double Drivetrain::ComputeOutput(data::drivetrain::ForwardOrTurn axis)
{
	double positionSetpoint = m_componentData->drivetrainData->getRelativePositionSetpoint(axis); // this will tell you how much further to go. i.e. if you have to go 5 units further forward it will return 5
	
//	AsyncPrinter::Printf("relative position setpoin: %lf\n", positionSetpoint);
	
	double velocitySetpoint = m_componentData->drivetrainData->getVelocitySetpoint(axis);
	double rawOutput 		= m_componentData->drivetrainData->getOpenLoopOutput(axis);
	
	switch (m_componentData->drivetrainData->getControlMode(axis))
	{
	case data::drivetrain::POSITION_CONTROL:
		m_PIDs[POSITION][axis].setInput(0.0);//we're always at our current position! :D -BA
		m_PIDs[POSITION][axis].setSetpoint(positionSetpoint);
		velocitySetpoint = m_PIDs[POSITION][axis].update( 1.0 / RobotConfig::LOOP_RATE);
		if (fabs(velocitySetpoint) > m_componentData->drivetrainData->getPositionControlMaxSpeed(axis))
			velocitySetpoint = Util::Sign(velocitySetpoint) * m_componentData->drivetrainData->getPositionControlMaxSpeed(axis);
		//fall through the switch
	case data::drivetrain::VELOCITY_CONTROL:
		//1.0e-2
		if (fabs(velocitySetpoint) < 2.0E-2)
			m_PIDs[VELOCITY][axis].setIIREnabled(true);
		else 
			m_PIDs[VELOCITY][axis].setIIREnabled(false);
		
		if (axis == data::drivetrain::FORWARD)
			m_PIDs[VELOCITY][axis].setInput(m_driveEncoders->getNormalizedForwardSpeed());
		else
			m_PIDs[VELOCITY][axis].setInput(m_driveEncoders->getNormalizedTurningSpeed());
			
		m_PIDs[VELOCITY][axis].setSetpoint(velocitySetpoint);
		
		rawOutput = m_PIDs[VELOCITY][axis].update(1.0 / RobotConfig::LOOP_RATE);
		break;
	case data::drivetrain::OPEN_LOOP:
		break;
	}
	return rawOutput;
}


void Drivetrain::enabledPeriodic()
{
	double fwdOutput = ComputeOutput(data::drivetrain::FORWARD); //positive means forward
	double turnOutput = ComputeOutput(data::drivetrain::TURN);   //positive means turning counter-clockwise. Matches the way driveencoders work.
	
//	static int e = 0;
//	if(++e % 2 == 0)
//		AsyncPrinter::Printf("fwd: %.2f, turn %.2f, fwd %.2f, turn %.2f\n", fwdOutput, turnOutput, m_driveEncoders->getNormalizedForwardSpeed(), m_driveEncoders->getNormalizedTurningSpeed());
	
	double leftOutput = fwdOutput - turnOutput;
	double rightOutput = fwdOutput + turnOutput;
	
	Util::Clamp<double>(leftOutput, -1.0, 1.0);
	Util::Clamp<double>(rightOutput, -1.0, 1.0);
	
	static int e = 0;
	if (++e % 50 == 0)
	{
//		AsyncPrinter::Printf("turnPos:%.4f\n",m_driveEncoders->getTurnAngle());
	}
//	AsyncPrinter::Printf("Encoders left: %.3f, %.2f\n", m_driveEncoders->getNormalizedSpeed(drivetrain::LEFT), DriverStation::GetInstance()->GetBatteryVoltage());
//	AsyncPrinter::Printf("Encoders right: %.3f\n", m_driveEncoders->getNormalizedSpeed(drivetrain::RIGHT));
	if (DriverStation::GetInstance()->GetBatteryVoltage() < 7.0)
	{
		AsyncPrinter::Printf("Decreasing current\n");
		m_escs[LEFT]->DecrementMaxVDiff();
		m_escs[RIGHT]->DecrementMaxVDiff();
	}
	else
	{
		m_escs[LEFT]->IncrementMaxVDiff();
		m_escs[RIGHT]->IncrementMaxVDiff();
	}
	m_escs[LEFT]->SetDutyCycle(leftOutput);
	m_escs[RIGHT]->SetDutyCycle(rightOutput);
	
	m_componentData->drivetrainData->serviceOperationSemaphores();
}


void Drivetrain::disabledPeriodic()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[RIGHT]->SetDutyCycle(0.0);
}

void Drivetrain::onDisable()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[RIGHT]->SetDutyCycle(0.0);
}

void Drivetrain::onEnable()
{
	//Nous ne faison rien ici maintenant
}

void Drivetrain::Configure()
{
	ConfigurePIDObject(&m_PIDs[VELOCITY][TURN], "velocity_turn", true);
	ConfigurePIDObject(&m_PIDs[VELOCITY][FORWARD], "velocity_fwd", true);

	ConfigurePIDObject(&m_PIDs[POSITION][TURN], "position_turn", false);
	ConfigurePIDObject(&m_PIDs[POSITION][FORWARD], "position_fwd", false);
}

void Drivetrain::Log()
{
	
}

void Drivetrain::ConfigurePIDObject(PID *pid, std::string objName, bool feedForward)
{
	double p = m_config->Get<double>(Component::GetName(), objName + "_P", 2.0);
	double i = m_config->Get<double>(Component::GetName(), objName + "_I", 0.0);
	double d = m_config->Get<double>(Component::GetName(), objName + "_D", 0.0);
	
	pid->setParameters(p, i, d, 1.0, 0.87, feedForward);//super high decay, this makes it just like a filter. If you want it to act more like an integral you reduce the decay. This must be tuned. 
}
