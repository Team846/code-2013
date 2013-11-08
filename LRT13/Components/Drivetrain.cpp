#include "Drivetrain.h"
#include "../Config/ConfigManager.h"
#include "../Config/RobotConfig.h"
#include "../Sensors/DriveEncoders.h"
#include "../Utils/Util.h"
#include "../SpeedController/ESC.h"
#include "../Config/DriverStationConfig.h"

//#define MOTION_PROFILE
#define ALTERNATE_VELOCITY

using namespace data;
using namespace drivetrain;

Drivetrain::Drivetrain() :
	Component("Drivetrain", DriverStationConfig::DigitalIns::DRIVETRAIN, true),
			m_driveEncoders(DriveEncoders::GetInstance()),
			m_log_file("/Drivetrain.csv")
{
	m_escs[LEFT] = new ESC(RobotConfig::CAN::LEFT_DRIVE_A,
			RobotConfig::CAN::LEFT_DRIVE_B,
			m_driveEncoders->getEncoder(data::drivetrain::LEFT), "left");
	m_escs[RIGHT] = new ESC(RobotConfig::CAN::RIGHT_DRIVE_A,
			RobotConfig::CAN::RIGHT_DRIVE_B,
			m_driveEncoders->getEncoder(data::drivetrain::RIGHT), "right");

	lastSpeed[LEFT] = 0.0;
	lastSpeed[RIGHT] = 0.0;

	m_profiles[FORWARD] = new TrapezoidProfile(1.0, 1.0);
	m_profiles[TURN] = new TrapezoidProfile(1.0, 1.0);
	m_profiled[FORWARD] = new ProfiledPID(m_profiles[FORWARD]);
	m_profiled[TURN] = new ProfiledPID(m_profiles[TURN]);
	m_scale = 1.0;
	m_arcGain = 33.0;
	table = NetworkTable::GetTable("RobotData");
}

Drivetrain::~Drivetrain()
{

}

double Drivetrain::ComputeOutput(data::drivetrain::ForwardOrTurn axis)
{
#ifndef MOTION_PROFILE
	double positionSetpoint =
			m_componentData->drivetrainData->getRelativePositionSetpoint(axis); // this will tell you how much further to go. i.e. if you have to go 5 units further forward it will return 5
#else
	double positionSetpoint = m_componentData->drivetrainData->getAbsolutePositionSetpoint(axis);
#endif

	double velocitySetpoint =
			m_componentData->drivetrainData->getVelocitySetpoint(axis);
	double rawOutput = m_componentData->drivetrainData->getOpenLoopOutput(axis);

	switch (m_componentData->drivetrainData->getControlMode(axis))
	{
	case data::drivetrain::POSITION_CONTROL:
#ifndef MOTION_PROFILE
		if (!m_componentData->drivetrainData->syncingArc() || axis == FORWARD)
		{
			m_PIDs[POSITION][axis].setInput(0.0); // We're always at our current position! :D -BA
			m_PIDs[POSITION][axis].setSetpoint(positionSetpoint);
			velocitySetpoint = m_PIDs[POSITION][axis].update(
					1.0 / RobotConfig::LOOP_RATE);
			if (fabs(velocitySetpoint)
					> m_componentData->drivetrainData->getPositionControlMaxSpeed(
							axis))
				velocitySetpoint
						= Util::Sign(velocitySetpoint)
								* m_componentData->drivetrainData->getPositionControlMaxSpeed(
										axis);
		}
		else // Turn control in arc syncing mode
		{
			m_PIDs[POSITION][axis].setInput(m_componentData->drivetrainData->getRelativePositionSetpoint(FORWARD)
					/ (m_componentData->drivetrainData->getAbsolutePositionSetpoint(FORWARD)
							- m_componentData->drivetrainData->getPositionControlStartingPosition(FORWARD))
					- m_componentData->drivetrainData->getRelativePositionSetpoint(TURN)
					/ (m_componentData->drivetrainData->getAbsolutePositionSetpoint(TURN)
							- m_componentData->drivetrainData->getPositionControlStartingPosition(TURN))); // Turn vs. forward proportion difference
			m_PIDs[POSITION][axis].setSetpoint(0.0);
			
//			velocitySetpoint = ((m_componentData->drivetrainData->getAbsolutePositionSetpoint(TURN)
//					- m_componentData->drivetrainData->getPositionControlStartingPosition(TURN))
//					/ ((m_componentData->drivetrainData->getAbsolutePositionSetpoint(FORWARD)
//					- m_componentData->drivetrainData->getPositionControlStartingPosition(FORWARD))
//					/ (m_componentData->drivetrainData->getVelocitySetpoint(FORWARD) * m_driveEncoders->getMaxSpeed())))
//					/ m_driveEncoders->getMaxTurnRate(); // Match turn rate to forward rate
			
			/* 
			 * To get a turn velocity from a forward velocity and a desired arc radius, use the following formula:
			 * Turn = Forward * Robot Width / (2 * Desired Radius),
			 * derived from arc radius formula (Desired Radius = Robot Width / (1 - Slower Wheel / Faster Wheel) - Robot Width / 2,
			 * where Slower Wheel = Forward - Turn and Faster Wheel = Forward + Turn).
			 * Turn is always the same sign as Forward, so add a sign to velocitySetpoint.
			 * -RC 10/16/2013
			 */
			double radius = fabs((m_componentData->drivetrainData->getAbsolutePositionSetpoint(FORWARD)
					- m_componentData->drivetrainData->getPositionControlStartingPosition(FORWARD))
					/ ((m_componentData->drivetrainData->getAbsolutePositionSetpoint(TURN)
					- m_componentData->drivetrainData->getPositionControlStartingPosition(TURN)) * acos(-1) / 180.0)); // Radius = Arc Length (distance setpoint) / Central Angle (angle setpoint to radians)
			velocitySetpoint = m_componentData->drivetrainData->getVelocitySetpoint(FORWARD)
					* RobotConfig::ROBOT_WIDTH / (2 * radius); // Match turn rate to forward rate
			velocitySetpoint *= Util::Sign(m_componentData->drivetrainData->getAbsolutePositionSetpoint(TURN)
					- m_componentData->drivetrainData->getPositionControlStartingPosition(TURN))
					* Util::Sign(m_componentData->drivetrainData->getAbsolutePositionSetpoint(FORWARD)
					- m_componentData->drivetrainData->getPositionControlStartingPosition(FORWARD));
			AsyncPrinter::Printf("Turn velocity: %f %f %f %f %f %f\n", velocitySetpoint, m_componentData->drivetrainData->getAbsolutePositionSetpoint(FORWARD)
					- m_componentData->drivetrainData->getPositionControlStartingPosition(FORWARD), m_componentData->drivetrainData->getAbsolutePositionSetpoint(TURN)
					- m_componentData->drivetrainData->getPositionControlStartingPosition(TURN), m_driveEncoders->getWheelDist(LEFT), m_driveEncoders->getWheelDist(RIGHT), radius);
			
			velocitySetpoint += m_arcGain / m_PIDs[POSITION][axis].getProportionalGain() * Util::Sign(m_componentData->drivetrainData->getAbsolutePositionSetpoint(TURN)
					- m_componentData->drivetrainData->getPositionControlStartingPosition(TURN)) * m_PIDs[POSITION][axis].update(
					1.0 / RobotConfig::LOOP_RATE); // Correction for turn vs. forward proportion difference
			AsyncPrinter::Printf("Turn velocitySetpoint: %f\n", velocitySetpoint);
		}
#else
		m_profiled[axis]->setInput(m_componentData->drivetrainData->getCurrentPos(axis) - m_componentData->drivetrainData->getPositionControlStartingPosition(axis));
		if (m_componentData->drivetrainData->isPositionSetpointChanged(axis))
		{
			m_profiles[axis]->updateValues(m_componentData->drivetrainData->getPositionControlMaxSpeed(
						axis) * (axis == FORWARD ? m_driveEncoders->getMaxSpeed() : m_driveEncoders->getMaxTurnRate()), m_timeToMax[axis]);
			m_profiled[axis]->setSetpoint(positionSetpoint - m_componentData->drivetrainData->getPositionControlStartingPosition(axis));
			m_componentData->drivetrainData->setPositionSetpointChanged(axis, false);
		}
		velocitySetpoint = m_profiled[axis]->update(
				1.0 / RobotConfig::LOOP_RATE) + m_profiles[axis]->getVelocity() / (axis == FORWARD ? m_driveEncoders->getMaxSpeed() : m_driveEncoders->getMaxTurnRate());
//		if (fabs(velocitySetpoint)
//				> m_componentData->drivetrainData->getPositionControlMaxSpeed(
//						axis))
//			velocitySetpoint
//					= Util::Sign(velocitySetpoint)
//							* m_componentData->drivetrainData->getPositionControlMaxSpeed(
//									axis);
#endif
		m_componentData->drivetrainData->setVelocitySetpoint(axis, velocitySetpoint);
#ifdef ALTERNATE_VELOCITY
		rawOutput = velocitySetpoint;
		break;
#endif
		//fall through the switch
	case data::drivetrain::VELOCITY_CONTROL:
#ifndef ALTERNATE_VELOCITY
		//1.0e-2
		if (fabs(velocitySetpoint) < 2.0E-2)
			m_PIDs[VELOCITY][axis].setIIREnabled(true);
		else
			m_PIDs[VELOCITY][axis].setIIREnabled(false);

		if (axis == data::drivetrain::FORWARD)
			m_PIDs[VELOCITY][axis].setInput(
					m_driveEncoders->getNormalizedForwardSpeed());
		else
			m_PIDs[VELOCITY][axis].setInput(
					m_driveEncoders->getNormalizedTurningSpeed());

		m_PIDs[VELOCITY][axis].setSetpoint(velocitySetpoint);

		rawOutput = m_PIDs[VELOCITY][axis].update(1.0 / RobotConfig::LOOP_RATE);
#else
		rawOutput = velocitySetpoint;
#endif
		break;
	case data::drivetrain::OPEN_LOOP:
		break;
	}
	return rawOutput;
}

double Drivetrain::ComputeSide(data::drivetrain::Side side, double forward, double turn)
{
	double setpoint = 0.0;
	if (side == LEFT)
		setpoint = forward - turn;
	else if (side == RIGHT)
		setpoint = forward + turn;
	if (fabs(setpoint) < 0.02)
		m_PIDs[VELOCITY][side - 2].setIIREnabled(true);
	else
		m_PIDs[VELOCITY][side - 2].setIIREnabled(false);

	m_PIDs[VELOCITY][side - 2].setInput(
			m_driveEncoders->getNormalizedSpeed(side));
	m_PIDs[VELOCITY][side - 2].setSetpoint(setpoint);

	return m_PIDs[VELOCITY][side - 2].update(1.0 / RobotConfig::LOOP_RATE);
}

void Drivetrain::enabledPeriodic()
{
	double fwdOutput = ComputeOutput(data::drivetrain::FORWARD); //positive means forward
	double turnOutput = ComputeOutput(data::drivetrain::TURN); //positive means turning counter-clockwise. Matches the way driveencoders work.

	//	static int e = 0;
	//	if(++e % 2 == 0)
	//		AsyncPrinter::Printf("fwd: %.2f, turn %.2f, fwd %.2f, turn %.2f\n", fwdOutput, turnOutput, m_driveEncoders->getNormalizedForwardSpeed(), m_driveEncoders->getNormalizedTurningSpeed());

#ifndef ALTERNATE_VELOCITY
	double leftOutput = fwdOutput - turnOutput;
	double rightOutput = fwdOutput + turnOutput;
#else
	double leftOutput = ComputeSide(LEFT, fwdOutput, turnOutput);
	double rightOutput = ComputeSide(RIGHT, fwdOutput, turnOutput);
#endif

	Util::Clamp<double>(leftOutput, -1.0, 1.0);
	Util::Clamp<double>(rightOutput, -1.0, 1.0);

	static int e = 0;
	if (++e % 50 == 0)
	{
		//		AsyncPrinter::Printf("turnPos:%.4f\n",m_driveEncoders->getTurnAngle());
	}
	table->PutNumber("LeftEncoder",
			m_driveEncoders->getNormalizedSpeed(drivetrain::LEFT));
	table->PutNumber("RightEncoder",
			m_driveEncoders->getNormalizedSpeed(drivetrain::RIGHT));
	table->PutNumber("TurnTicks", m_driveEncoders->getTurnTicks());
	static int ticks = 0;
//	m_log_file << (double) (ticks++ / 50.0) << "," << m_driveEncoders->getNormalizedSpeed(drivetrain::LEFT) - lastSpeed[LEFT]
//	<< "," << fabs(leftOutput - m_driveEncoders->getNormalizedSpeed(drivetrain::LEFT)) << "," << m_driveEncoders->getNormalizedSpeed(drivetrain::RIGHT) - lastSpeed[RIGHT]
//	<< "," << fabs(rightOutput - m_driveEncoders->getNormalizedSpeed(drivetrain::RIGHT)) << "\n";
	if (m_componentData->drivetrainData->getControlMode(FORWARD) == POSITION_CONTROL)
		m_log_file << (double) (ticks++ / 50.0) << "," << m_driveEncoders->getNormalizedForwardSpeed() * m_driveEncoders->getMaxSpeed() << "," << m_profiles[FORWARD]->getVelocity() << ',' << m_componentData->drivetrainData->getCurrentPos(FORWARD) - m_componentData->drivetrainData->getPositionControlStartingPosition(FORWARD) << ',' << m_profiles[FORWARD]->getOutput() << '\n';
	lastSpeed[LEFT] = m_driveEncoders->getNormalizedSpeed(drivetrain::LEFT);
	lastSpeed[RIGHT] = m_driveEncoders->getNormalizedSpeed(drivetrain::RIGHT);
	
	if (m_componentData->drivetrainData->shouldOverrideForwardCurrentLimit())
	{
		m_escs[LEFT]->SetForwardCurrentLimit(m_componentData->drivetrainData->getForwardCurrentLimit());
		m_escs[RIGHT]->SetForwardCurrentLimit(m_componentData->drivetrainData->getForwardCurrentLimit());
	}
	else
	{
		ConfigureForwardCurrentLimit();
	}
	if (m_componentData->drivetrainData->shouldOverrideReverseCurrentLimit())
	{
		m_escs[LEFT]->SetReverseCurrentLimit(m_componentData->drivetrainData->getReverseCurrentLimit());
		m_escs[RIGHT]->SetReverseCurrentLimit(m_componentData->drivetrainData->getReverseCurrentLimit());
	}
	else
	{
		ConfigureReverseCurrentLimit();
	}
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
	m_escs[LEFT]->SetDutyCycle(leftOutput * m_scale);
	m_escs[RIGHT]->SetDutyCycle(rightOutput * m_scale);

	m_componentData->drivetrainData->serviceOperationSemaphores();
}

void Drivetrain::disabledPeriodic()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[RIGHT]->SetDutyCycle(0.0);
	table->PutNumber("TurnTicks", m_driveEncoders->getTurnTicks());
//	printf("TurnTIcks: %d\n", m_driveEncoders->getTurnTicks());
}

void Drivetrain::onDisable()
{
	m_escs[LEFT]->SetDutyCycle(0.0);
	m_escs[RIGHT]->SetDutyCycle(0.0);
	m_log_file.flush();
	if (m_log_file.is_open())
		m_log_file.close();
}

void Drivetrain::onEnable()
{
	if (!m_log_file.is_open())
		m_log_file.open("/Drivetrain.csv");
}

void Drivetrain::Configure()
{
	ConfigurePIDObject(&m_PIDs[VELOCITY][TURN], "velocity_turn", true);
	ConfigurePIDObject(&m_PIDs[VELOCITY][FORWARD], "velocity_fwd", true);

#ifdef MOTION_PROFILE
	ConfigurePIDObject((PID*)m_profiled[TURN], "position_turn", false);
	ConfigurePIDObject((PID*)m_profiled[FORWARD], "position_fwd", false);
#else
	ConfigurePIDObject(&m_PIDs[POSITION][TURN], "position_turn", false);
	ConfigurePIDObject(&m_PIDs[POSITION][FORWARD], "position_fwd", false);
#endif

	ConfigureForwardCurrentLimit();
	ConfigureReverseCurrentLimit();
	
	m_scale = m_config->Get<double> (Component::GetName(), "speed_scale", 1.0);
	m_arcGain = m_config->Get<double> (Component::GetName(), "arc_gain", 33.0);
	m_timeToMax[FORWARD] = m_config->Get<double> (Component::GetName(), "time_to_max_forward", 1.0);
	m_timeToMax[TURN] = m_config->Get<double> (Component::GetName(), "time_to_max_turn", 1.0);
}

void Drivetrain::Log()
{

}

void Drivetrain::ConfigurePIDObject(PID *pid, std::string objName,
		bool feedForward)
{
	double p =
			m_config->Get<double> (Component::GetName(), objName + "_P", 2.0);
	double i =
			m_config->Get<double> (Component::GetName(), objName + "_I", 0.0);
	double d =
			m_config->Get<double> (Component::GetName(), objName + "_D", 0.0);

	pid->setParameters(p, i, d, 1.0, 0.87, feedForward);//super high decay, this makes it just like a filter. If you want it to act more like an integral you reduce the decay. This must be tuned. 
}

void Drivetrain::ConfigureForwardCurrentLimit()
{
	m_escs[LEFT]->SetForwardCurrentLimit(m_config->Get<float>(Component::GetName(), "forwardCurrentLimit", 50.0 / 100.0));
	m_escs[RIGHT]->SetForwardCurrentLimit(m_config->Get<float>(Component::GetName(), "forwardCurrentLimit", 50.0 / 100.0));
}

void Drivetrain::ConfigureReverseCurrentLimit()
{
	m_escs[LEFT]->SetReverseCurrentLimit(m_config->Get<float>(Component::GetName(), "reverseCurrentLimit", 50.0 / 100.0));
	m_escs[RIGHT]->SetReverseCurrentLimit(m_config->Get<float>(Component::GetName(), "reverseCurrentLimit", 50.0 / 100.0));
}
