#include "DriveEncoders.h"
#include "..\Config\RobotConfig.h"
#include "..\Config\ConfigManager.h"

DriveEncoders* DriveEncoders::m_instance = NULL;

using namespace data::drivetrain;

DriveEncoders& DriveEncoders::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new DriveEncoders();
	return *m_instance;
}

DriveEncoders::DriveEncoders() :
	m_configsection("DriveEncoders")
{
	m_encoders[LEFT] = new LRTEncoder("Left Drive Encoder",
			RobotConfig::digital_io::LEFT_DRIVE_ENCODER_A,
			RobotConfig::digital_io::LEFT_DRIVE_ENCODER_B);
	m_encoders[RIGHT] = new LRTEncoder("Right Drive Encoder",
			RobotConfig::digital_io::RIGHT_DRIVE_ENCODER_A,
			RobotConfig::digital_io::RIGHT_DRIVE_ENCODER_B);

	// want to stay with ticks/second
	m_encoders[LEFT]->SetDistancePerPulse(1);
	m_encoders[RIGHT]->SetDistancePerPulse(1);

	m_encoders[LEFT]->Start();
	m_encoders[RIGHT]->Start();
	printf("Construct Drive Encoders\n");

	MAX_ENCODER_RATE = MAX_TURNING_RATE
			= PULSES_PER_REVOLUTION = TICKS_PER_FULL_TURN = WHEEL_DIAMETER = 0;

	Configure();
}

DriveEncoders::~DriveEncoders()
{
	delete m_encoders[LEFT];
	delete m_encoders[RIGHT];
}

void DriveEncoders::Configure()
{
	PULSES_PER_REVOLUTION = m_config->Get<double> (m_configsection,
			"pulses_per_revolution", 360.0);
	MAX_ENCODER_RATE = m_config->Get<double> (m_configsection,
			"high_gear_encoder_rate", 6100);
	MAX_TURNING_RATE = m_config->Get<double> (m_configsection,
			"max_turning_rate", 6100.0);
	TICKS_PER_FULL_TURN = m_config->Get<double> (m_configsection,
			"ticks_per_full_turn", 2288.3 * 180.0 / 165.0 * 2.0);
	WHEEL_DIAMETER = m_config->Get<double> (m_configsection, "wheel_diameter",
			4.0); // inches

	m_encoders[LEFT]->Start();
	m_encoders[RIGHT]->Start();
}

void DriveEncoders::Log()
{
#if LOGGING_ENABLED
	SmartDashboard * sdb = SmartDashboard::GetInstance();
	sdb->PutDouble("Robot Drive Speed", getNormalizedForwardSpeed());
	sdb->PutDouble("Robot Turning Speed", getNormalizedTurningMotorSpeed());
	sdb->PutDouble("Robot Drive Distance", getRobotDist());
	sdb->PutDouble("Robot Turn Angle", getTurnAngle());
#endif
}

double DriveEncoders::GetRawForwardSpeed()
{
	return (m_encoders[LEFT]->GetRate() + m_encoders[RIGHT]->GetRate()) / 2;
}

double DriveEncoders::getNormalizedForwardSpeed()
{
	return GetRawForwardSpeed() / getMaxEncoderRate();
}

double DriveEncoders::getRawTurningSpeed()
{
	return m_encoders[RIGHT]->GetRate() - m_encoders[LEFT]->GetRate();
}

double DriveEncoders::getNormalizedTurningSpeed()
{
	return getRawTurningSpeed() / MAX_TURNING_RATE;
}

double DriveEncoders::getRobotDist()
{
	return (getWheelDist(LEFT) + getWheelDist(RIGHT)) / 2;
}

int DriveEncoders::getTurnTicks()
{
	// CCW is positive, CW is negative
	return m_encoders[RIGHT]->Get() - m_encoders[LEFT]->Get();
}

double DriveEncoders::getTurnRevolutions()
{
	return getTurnTicks() / TICKS_PER_FULL_TURN;
}

double DriveEncoders::getTurnAngle()
{
	return getTurnRevolutions() * 360.0;
}

///************* Distance functions **************************************/
double DriveEncoders::getWheelDist(Side side)
{
	// pulses / ( pulses / revolution ) * distance / revolution = inch distance
	LRTEncoder * e = (side == LEFT ? m_encoders[LEFT] : m_encoders[RIGHT]);
	double dist = (double) ((e->Get() * 1.0) / PULSES_PER_REVOLUTION
			* WHEEL_DIAMETER * PI);
	return dist;
}

double DriveEncoders::getNormalizedSpeed(Side side)
{
	return m_encoders[side]->GetRate() / MAX_ENCODER_RATE;
}

LRTEncoder* DriveEncoders::getEncoder(Side side)
{
	return m_encoders[side];
}


double DriveEncoders::getMaxEncoderRate()
{
	return MAX_ENCODER_RATE;
}
