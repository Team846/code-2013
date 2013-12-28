#include "DriveESC.h"
#include "AsyncCANJaguar.h"
#include "../Sensors/DriveEncoders.h"

DriveESC::DriveESC(LRTSpeedController *esc, LRTEncoder* encoder, string name) :
	Loggable(name),
	m_encoder(encoder),
	m_controller1(esc)
{
	m_controller1->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	
	m_dutyCycle = 0.0;
	m_cycle_count = 0;
	m_shouldBrakeThisCycle = false;
	
	m_forwardCurrentLimit = 50.0 / 100.0;
	m_reverseCurrentLimit = 50.0 / 100.0;
}

DriveESC::DriveESC(LRTSpeedController *esc1, LRTSpeedController *esc2, LRTEncoder* encoder, string name) :
	Loggable(name),
	m_encoder(encoder),
	m_controller1(esc1),
	m_controller2(esc2)
{
	m_controller1->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	m_controller2->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);

	m_dutyCycle = 0.0;
	m_cycle_count = 0;
	m_shouldBrakeThisCycle = false;
	
	m_forwardCurrentLimit = 50.0 / 100.0;
	m_reverseCurrentLimit = 50.0 / 100.0;
}

DriveESC::~DriveESC()
{
	DELETE(m_controller1);
	DELETE(m_controller2);
}

DriveESC::BrakeAndDutyCycle DriveESC::CalculateBrakeAndDutyCycle(float desired_speed, float current_speed)
{
	BrakeAndDutyCycle command;

	command.dutyCycle = 0.0;

	if (current_speed < 0)
	{
		command = CalculateBrakeAndDutyCycle(-desired_speed, -current_speed);
		command.dutyCycle = -command.dutyCycle;
		return command;
	}

	// Speed >= 0 at this point
	if (desired_speed >= current_speed) // Going faster
	{
		command.dutyCycle = desired_speed;
		command.braking = 0.0;
	}
	else // Slowing down
	{
		float error = desired_speed - current_speed; // error always <= 0

		if (desired_speed >= 0) // Braking is based on speed alone; reverse power unnecessary
		{
			command.dutyCycle = 0.0; // Must set 0 to brake

			if (current_speed > -error + 0.05)
				command.braking = -error / current_speed; // Speed always > 0
			else
				command.braking = 1.0;
		}
		else // Input < 0, braking with reverse power
		{
			command.braking = 0.0; // not braking
			command.dutyCycle = error / (1.0 + current_speed); // DutyCycle <= 0 because error <= 0
		}
	}

	return command;
}

void DriveESC::SetDutyCycle(float dutyCycle)
{
	double speed = m_encoder->GetRate()
				/ DriveEncoders::GetMaxEncoderRate();
	
	speed = Util::Clamp<double>(speed, -1, 1);
	
	dutyCycle = DitheredBraking(dutyCycle, speed);
	dutyCycle = CurrentLimit(dutyCycle, speed);
	m_dutyCycle = dutyCycle;
	
	m_controller1->SetDutyCycle(m_dutyCycle);
	if (m_controller2 != NULL)
		m_controller2->SetDutyCycle(m_dutyCycle);
}

float DriveESC::DitheredBraking(float dutyCycle, float speed)
{
	dutyCycle = Util::Clamp<float>(dutyCycle, -1.0, 1.0);
	BrakeAndDutyCycle command = CalculateBrakeAndDutyCycle(dutyCycle, speed);
	command.dutyCycle = Util::Clamp<float>(command.dutyCycle, -1.0, 1.0);

	// Default to coast
	m_controller1->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	if (m_controller2 != NULL)
		m_controller2->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Coast);
	
	if (fabs(command.dutyCycle) < 1E-4) //brake only when duty cycle = 0
	{
		dutyCycle = 0.0;

		// cycle count ranges from 0 to 8
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
		m_shouldBrakeThisCycle = ditherPattern[brake_level] & (1 << m_cycle_count);

		if (m_shouldBrakeThisCycle)
		{
			m_controller1->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Brake);
			if (m_controller2 != NULL)
				m_controller2->ConfigNeutralMode(LRTSpeedController::kNeutralMode_Brake);
		}
	}

	return command.dutyCycle;
}

float DriveESC::CurrentLimit(float dutyCycle, float speed)
{
	if (speed < 0)
	{
		return -CurrentLimit(-dutyCycle, -speed);
	}
	// At this point speed >= 0
	if (dutyCycle > speed) // Current limit accelerating
	{
		dutyCycle = Util::Min(dutyCycle, speed + m_forwardCurrentLimit);
	}
	else if (dutyCycle < 0) // Current limit reversing direction
	{
		float limitedDutyCycle = -m_reverseCurrentLimit / (1.0 + speed); // speed >= 0 so dutyCycle < -currentLimit
		dutyCycle = Util::Max(dutyCycle, limitedDutyCycle); // Both are negative
	}
	
	return dutyCycle;
}

void DriveESC::SetForwardCurrentLimit(float limit)
{
	m_forwardCurrentLimit = limit;
}

void DriveESC::SetReverseCurrentLimit(float limit)
{
	m_reverseCurrentLimit = limit;
}

void DriveESC::Disable()
{
	m_controller1->SetDutyCycle(0.0);
	if (m_controller2 != NULL)
		m_controller2->SetDutyCycle(0.0);
}

void DriveESC::ResetCache()
{
	if(dynamic_cast<AsyncCANJaguar*>(m_controller1) )
		dynamic_cast<AsyncCANJaguar*>(m_controller1)->ResetCache();
	if(dynamic_cast<AsyncCANJaguar*>(m_controller2))
		dynamic_cast<AsyncCANJaguar*>(m_controller2)->ResetCache();
}

void DriveESC::Log()
{
	LogToFile(&m_dutyCycle, "DutyCycle");
	LogToFile(&m_shouldBrakeThisCycle, "Brake");
}
