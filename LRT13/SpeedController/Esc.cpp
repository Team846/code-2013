#define MIN(a,b) ((a)>(b)?(b):(a))
#define MAX(a,b) ((a)<(b)?(b):(a))

#include "Esc.h"
#include "../Config/RobotConfig.h"

//Talons
ESC::ESC(int channel, LRTEncoder* encoder, string name, int brakeChannel) :
	m_name(name)
{
	m_encoder = encoder;
	m_controller1 = new LRTTalon(channel, name.c_str(), brakeChannel);
	m_controller2 = NULL;
	m_cycle_count = 0;
	m_delta_voltage_limit = 13.0;
	maxVDiff = 2.0;
}

ESC::ESC(int channelA, int channelB, int brakeChannelA, int brakeChannelB, LRTEncoder* encoder, string name) :
	m_name(name)
{
	m_encoder = encoder;
	namea = name + "A";
	nameb = name + "B";
	
	m_controller1 = new LRTTalon(channelA, namea.c_str(), brakeChannelA);
	m_controller2 = new LRTTalon(channelB, nameb.c_str(), brakeChannelB);

	m_controller1->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	m_controller2->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);

	m_cycle_count = 0;
	m_delta_voltage_limit = 13.0;
	printf("Constructed ESC: %s\n", name.c_str());
}
//Jags
/************************** Esc Class ********************/
ESC::ESC(int channel, LRTEncoder *encoder, string name) :
	m_name(name)
{
	m_encoder = encoder;
	m_jag1 = new AsyncCANJaguar(channel, name.c_str());
	m_jag2 = NULL;
	m_cycle_count = 0;
	m_delta_voltage_limit = 13.0;
	maxVDiff = 2.0;
}

ESC::ESC(int channelA, int channelB, LRTEncoder* encoder, string name) :
	m_name(name)
{
	m_encoder = encoder;
	namea = name + "A";
	nameb = name + "B";
	
	m_jag1 = new AsyncCANJaguar(channelA, namea.c_str());
	m_jag2 = new AsyncCANJaguar(channelB, nameb.c_str());

	m_jag1->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	m_jag2->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);

	m_cycle_count = 0;
	m_delta_voltage_limit = 13.0;
	printf("Constructed ESC: %s\n", name.c_str());
}


ESC::~ESC()
{
	DELETE(m_controller1);
	DELETE(m_controller2);
}

void ESC::DecrementMaxVDiff()
{
	maxVDiff *= 0.99;
}

void ESC::IncrementMaxVDiff()
{
	maxVDiff *= 1.02;
	if (maxVDiff > 2.0)
		maxVDiff = 2.0;
}

void ESC::ResetMaxVDiff()
{
	maxVDiff = 2.0;
}

void ESC::Configure()
{
	string configSection("Esc");
}

ESC::brakeAndDutyCycle ESC::CalculateBrakeAndDutyCycle(float desired_speed,
		float current_speed)
{
	brakeAndDutyCycle command;

	command.dutyCycle = 0.0;

	if (current_speed < 0)
	{
		command = CalculateBrakeAndDutyCycle(-desired_speed, -current_speed);
		command.dutyCycle = -command.dutyCycle;
		return command;
	}

	// speed >= 0 at this point
	if (desired_speed >= current_speed) // trying to go faster
	{
		command.dutyCycle = desired_speed;
		command.braking = 0.0;
	}
	// trying to slow down
	else
	{
		float error = desired_speed - current_speed; // error always <= 0

		if (desired_speed >= 0) // braking is based on speed alone; reverse power unnecessary
		{
			command.dutyCycle = 0.0; // must set 0 to brake

			if (current_speed > -error + 0.05)
				command.braking = -error / current_speed; // speed always > 0
			else
				command.braking = 1.0;
		}
		else // input < 0; braking with reverse power
		{
			command.braking = 0.0; // not braking
			command.dutyCycle = error / (1.0 + current_speed); // dutyCycle <= 0 because error <= 0
		}
	}

	command.braking = fabs(command.braking);
	return command;
}

void ESC::SetDutyCycle(float dutyCycle)
{
	double speed = m_encoder->GetRate()
			/ DriveEncoders::GetInstance()->getMaxEncoderRate();
	

	speed = Util::Clamp<double>(speed, -1, 1);
	
	brakeAndDutyCycle command = CalculateBrakeAndDutyCycle(dutyCycle, speed);

	//	static int e = 0; 
	//	if ( ++e % 5 == 0)
	//		AsyncPrinter::Printf("\nDuty %.3f Speed %.3f\n", dutyCycle, speed);
	//	float origDutyCycle = dutyCycle;

	if (fabs(command.dutyCycle) < 1E-4) //brake only when duty cycle = 0
	{
		dutyCycle = 0.0;

		// cycleCount ranges from 0 to 8
		if (++m_cycle_count >= 8)
			m_cycle_count = 0;

		/*
		 * Each integer, corresponding to value, is a bitfield of 8 cycles
		 * Pattern N has N bits out of 8 set to true.
		 * 0: 0000 0000 = 0x00
		 * 1: 0000 0001 = 0x01
		 * 2: 0001 0001 = 0x11
		 * 3: 0010 0101 = 0x25
		 * 4: 0101 0101 = 0x55
		 * 5: 1101 0101 = 0xD5
		 * 6: 1110 1110 = 0xEE
		 * 7: 1111 1110 = 0xFE
		 * 8: 1111 1111 = 0xFF
		 */
		static const UINT8 ditherPattern[] =
		{ 0x00, 0x01, 0x11, 0x25, 0x55, 0xD5, 0xEE, 0xFE, 0xFF };

		int brake_level = (int) (fabs(command.braking) * 8);
		bool shouldBrakeThisCycle = ditherPattern[brake_level] & (1
				<< m_cycle_count);
//		AsyncPrinter::Printf("Braking: %d\n", shouldBrakeThisCycle);
		
		if (shouldBrakeThisCycle)
		{

			m_controller1->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Brake);
			if (m_controller2 != NULL)
				m_controller2->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Brake);

		}
		else
		{

			m_controller1->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
			if (m_controller2 != NULL)
				m_controller2->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);

		}
		//		AsyncPrinter::Printf("Braking\n");
	}

	dutyCycle = Util::Clamp<float>(dutyCycle, -1.0, 1.0);

	//	static int e = 0;
	//	if ((e++)%21 == 0)
	//		AsyncPrinter::Printf("In: %.3f out %.3f speed %.3f origspeed %.3f braking %.3f, %.3f max speed\n", origDutyCycle, command.dutyCycle, speed, (m_encoder->GetRate() / DriveEncoders::GetInstance().getMaxEncoderRate()) , command.braking, DriveEncoders::GetInstance().getMaxEncoderRate());

//#define CURRENT_LIMIT
#ifdef CURRENT_LIMIT
	// TODO: change delta voltage based on battery
	float dutyCycleLimitSameDir = fabs(speed) + maxVDiff;
	float dutyCycleLimitReverse = maxVDiff - fabs(speed);
	if (dutyCycleLimitReverse < 0)
		dutyCycleLimitReverse = 0.0;
#endif
#define LINEARIZE 1
#if LINEARIZE
#ifdef CURRENT_LIMIT
	if (speed > 0)
	{
		if (command.dutyCycle > 0)
		{
			m_controller1->SetDutyCycle(min(command.dutyCycle, dutyCycleLimitSameDir));
			if (m_controller2 != NULL)
				m_controller2->SetDutyCycle(min(command.dutyCycle, dutyCycleLimitSameDir));

		}
		else
		{

			m_controller1->SetDutyCycle(max(command.dutyCycle, -dutyCycleLimitReverse));
			if (m_controller2 != NULL)
				m_controller2->SetDutyCycle(max(command.dutyCycle, -dutyCycleLimitReverse));
		}
	}
	else
	{
		if (command.dutyCycle < 0)
		{

			m_controller1->SetDutyCy*Icle(max(command.dutyCycle, -dutyCycleLimitSameDir));
			if (m_controller2 != NULL)
				m_controller2->SetDutyCycle(max(command.dutyCycle, -dutyCycleLimitSameDir));

		}
		else
		{

			m_controller1->SetDutyCycle(min(command.dutyCycle, dutyCycleLimitReverse));
			if (m_controller2 != NULL)
				m_controller2->SetDutyCycle(min(command.dutyCycle, dutyCycleLimitReverse));

		}
	}
#else

	m_controller1->SetDutyCycle(command.dutyCycle);
	if (m_controller2 != NULL)
		m_controller2->SetDutyCycle(command.dutyCycle);
#endif
#endif
//	AsyncPrinter::Printf("Speed %.2f out: %.2f, braking %.2f\n", speed, command.dutyCycle, command.braking);
}
	
void ESC::Disable()
{
	m_controller1->SetDutyCycle(0.0);
	if (m_controller2 != NULL)
		m_controller2->SetDutyCycle(0.0);

}

void ESC::ResetCache()
{
	if(dynamic_cast<AsyncCANJaguar*>(m_controller1) )
		dynamic_cast<AsyncCANJaguar*>(m_controller1)->ResetCache();
	if(dynamic_cast<AsyncCANJaguar*>(m_controller2))
		dynamic_cast<AsyncCANJaguar*>(m_controller2)->ResetCache();

}

