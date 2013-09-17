#include "TeleopInputs.h"
#include "DebouncedJoystick.h"
#include "../ComponentData/ComponentData.h"
#include "../Config/DriverStationConfig.h"
#include "../Config/RobotConfig.h"
#include "../Utils/AsyncPrinter.h"
#include "AutoActions.h"
#include "../ComponentData/ClimberData.h"
#include "../ComponentData/ShooterData.h"
#include "../ComponentData/CollectorData.h"
#include <cmath>

using namespace data;
using namespace shooter;
using namespace climber;

TeleopInputs::TeleopInputs(char * taskName, INT32 priority)
//:
//	SynchronizedProcess(taskName, priority)
{
	m_componentData = ComponentData::GetInstance();
	m_driver_stick = new DebouncedJoystick(
			DriverStationConfig::JoystickConfig::DRIVER_STICK_PORT,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_BUTTONS,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_AXES);
	m_operator_stick = new DebouncedJoystick(
			DriverStationConfig::JoystickConfig::OPERATOR_STICK_PORT,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_BUTTONS,
			DriverStationConfig::JoystickConfig::NUM_JOYSTICK_AXES);
	m_driver_wheel = new DebouncedJoystick(
			DriverStationConfig::JoystickConfig::DRIVER_WHEEL_PORT,
			DriverStationConfig::JoystickConfig::NUM_WHEEL_BUTTONS,
			DriverStationConfig::JoystickConfig::NUM_WHEEL_AXES);

	m_autoActions = AutoActions::Instance();
	m_componentData->drivetrainData->setZeroHeading();
	r = 0, g = 64, b = 127;
	lastPressed = false;
	continuous = false;
	doubleTimeout = 0;
	holdTicks = 0;
}

TeleopInputs::~TeleopInputs()
{
	DELETE(m_driver_stick);
	DELETE(m_operator_stick);
	DELETE(m_driver_wheel);

	DELETE(m_autoActions);
}

INT32 TeleopInputs::Tick()
{
	m_driver_stick->Update();
	m_operator_stick->Update();
	m_driver_wheel->Update();
	Update();

	return 0;
}

void TeleopInputs::Update()
{
	RobotData::RobotState current_state = RobotData::GetCurrentState();

	/************************Drivetrain************************/

	//#define USEOPENLOOP
	// Use velocity control in teleoperated mode
	if (!m_driver_stick->IsButtonDown(
			DriverStationConfig::JoystickButtons::VECTOR_DRIVE))
	{
#ifdef USEOPENLOOP

		m_componentData->drivetrainData->setControlMode(FORWARD, OPEN_LOOP);
		m_componentData->drivetrainData->setControlMode(TURN, OPEN_LOOP);
#else
		m_componentData->drivetrainData->setControlMode(FORWARD,
				VELOCITY_CONTROL);
		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
	}
	else
	{
		m_componentData->drivetrainData->setControlMode(FORWARD,
				VELOCITY_CONTROL);
		m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
	}
#endif
	static bool lastStop = false;
	if (current_state == RobotData::TELEOP)
	{
		if (m_driver_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::RESET_ZERO))
		{
			m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, 0.0);
			m_componentData->drivetrainData->setZeroHeading();
		}
		else
		{
			if (!m_driver_stick->IsButtonDown(
					DriverStationConfig::JoystickButtons::VECTOR_DRIVE))
			{
				//#define DUAL_STICK
#ifdef DUAL_STICK
				double turn = 0.0;
				double forward = 0.0;

				double left = pow(-m_driver_stick->GetAxis(Joystick::kYAxis),
						RobotConfig::Drive::THROTTLE_EXPONENT);
				double right = pow(-m_operator_stick->GetAxis(Joystick::kYAxis),
						RobotConfig::Drive::THROTTLE_EXPONENT);
				forward = (left + right) / 2;
				turn = (right - left) / 2;
				int sign = turn > 0 ? 1 : -1;

				if (fabs(turn) < RobotConfig::Drive::DEADBAND)
				turn = 0.0;
				else
				{
					turn -= sign * RobotConfig::Drive::DEADBAND;
					turn /= 1.0 - RobotConfig::Drive::DEADBAND;
				}
				double turnComposite = turn;//sign * turn * turn;
				// TODO try blended turning
#else
				double turn = 0.0;
				turn = -m_driver_wheel->GetAxis(Joystick::kXAxis);
				//			turn *= 2;

				int sign = turn > 0 ? 1 : -1;

				//			turn *= turn * sign;
				//turn = -m_driver_stick->GetAxis(Joystick::kZAxis);

				double forward = pow(
						-m_driver_stick->GetAxis(Joystick::kYAxis),
						RobotConfig::Drive::THROTTLE_EXPONENT);

				int signForward = forward > 0 ? 1 : -1;

				if (fabs(forward) < RobotConfig::Drive::DEADBAND)
					forward = 0.0;
				else
				{
					forward -= signForward * RobotConfig::Drive::DEADBAND;
					forward /= 1.0 - RobotConfig::Drive::DEADBAND;
				}

				//blending routine
				double absForward = fabs(forward); //to ensure correct arc when switching direction

				double blend = pow((1 - absForward),
						RobotConfig::Drive::BLEND_EXPONENT); //always between 0 and 1, raised to an exponent to adjust transition between in place and arc.

				const double turnInPlace = turn; //normal turn
				const double constRadiusTurn = turn * absForward; //arc turn

				double turnComposite = turnInPlace * (blend) + constRadiusTurn
						* (1 - blend); //blended function

				if (m_driver_wheel->IsButtonDown(
						DriverStationConfig::JoystickButtons::STOP_ROBOT))
				{
					forward = 0.0;
					turnComposite = 0.0;
					turn = 0.0;
					if (!lastStop)
					{
						stoppedForward
								= m_componentData->drivetrainData->getCurrentPos(
										FORWARD);
						stoppedTurn
								= m_componentData->drivetrainData->getCurrentPos(
										TURN);
					}
					m_componentData->drivetrainData->setControlMode(FORWARD,
							POSITION_CONTROL);
					m_componentData->drivetrainData->setRelativePositionSetpoint(
							FORWARD,
							stoppedForward
									- m_componentData->drivetrainData->getCurrentPos(
											FORWARD), 1.0);

					m_componentData->drivetrainData->setControlMode(TURN,
							POSITION_CONTROL);
					m_componentData->drivetrainData->setRelativePositionSetpoint(
							TURN,
							stoppedTurn
									- m_componentData->drivetrainData->getCurrentPos(
											TURN), 1.0);
					lastStop = true;
				}
				else
					lastStop = false;
#endif

				//				AsyncPrinter::Printf("turnComposite: %lf forward: %lf\n", turnComposite, forward);

#ifdef USEOPENLOOP
				m_componentData->drivetrainData->setOpenLoopOutput(FORWARD, forward);
				m_componentData->drivetrainData->setOpenLoopOutput(TURN, turnComposite);
#else
				//			static int oops = 0;
				//			if (++oops % 5 == 0)
				//				AsyncPrinter::Printf("fwd: %.2f\n", forward);

				//				static double lastForward;
				//				static double lastTurn;
				//				if (forward == 0.0)
				//				{
				//					if (forward != lastForward)
				//						stoppedForward = m_componentData->drivetrainData->getCurrentPos(FORWARD);
				//					m_componentData->drivetrainData->setControlMode(FORWARD, POSITION_CONTROL);
				//					m_componentData->drivetrainData->setRelativePositionSetpoint(FORWARD, stoppedForward - m_componentData->drivetrainData->getCurrentPos(FORWARD), 1.0);
				//				}
				//				if (turnComposite == 0.0)
				//				{
				//					if (turnComposite != lastTurn)
				//						stoppedTurn
				//								= m_componentData->drivetrainData->getCurrentPos(
				//										TURN);
				//					m_componentData->drivetrainData->setControlMode(TURN, POSITION_CONTROL);
				//					m_componentData->drivetrainData->setRelativePositionSetpoint(TURN, stoppedTurn - m_componentData->drivetrainData->getCurrentPos(TURN), 1.0);
				//				}
				//				lastForward = forward;
				//				lastTurn = turn;

				m_componentData->drivetrainData->setOpenLoopOutput(FORWARD,
						forward);
				m_componentData->drivetrainData->setOpenLoopOutput(TURN,
						turnComposite);
				//for when the climber is being used

				m_componentData->drivetrainData->setVelocitySetpoint(FORWARD,
						forward);
				m_componentData->drivetrainData->setVelocitySetpoint(TURN,
						turnComposite);
				//m_componentData->drivetrainData->setVelocitySetpoint(TURN, turn);
#endif
			}
			else
			{
				float desiredHeading = atan2(
						-m_driver_stick->GetAxis(Joystick::kXAxis),
						-m_driver_stick->GetAxis(Joystick::kYAxis)) * 180
						/ acos(-1); // radians to degrees
				float magnitude = sqrt(
						m_driver_stick->GetAxis(Joystick::kXAxis)
								* m_driver_stick->GetAxis(Joystick::kXAxis)
								+ m_driver_stick->GetAxis(Joystick::kYAxis)
										* m_driver_stick->GetAxis(
												Joystick::kYAxis));
				if (fabs(
						desiredHeading
								- m_componentData->drivetrainData->getCurrentHeading())
						> fabs(
								desiredHeading
										- m_componentData->drivetrainData->getCurrentHeading()
										+ 360))
					desiredHeading += 360;
				else if (fabs(
						desiredHeading
								- m_componentData->drivetrainData->getCurrentHeading())
						> fabs(
								desiredHeading
										- m_componentData->drivetrainData->getCurrentHeading()
										- 360))
					desiredHeading -= 360;
				AsyncPrinter::Printf("Vector drive: %f degrees, %f\n",
						desiredHeading, magnitude);
				AsyncPrinter::Printf("Current heading: %f\n",
						m_componentData->drivetrainData->getCurrentHeading());
				m_componentData->drivetrainData->setVelocitySetpoint(FORWARD,
						magnitude);
				m_componentData->drivetrainData->setRelativePositionSetpoint(
						TURN,
						magnitude == 0 ? 0.0
								: desiredHeading
										- m_componentData->drivetrainData->getCurrentHeading(),
						magnitude + 0.5);
			}
		}

		//	AsyncPrinter::Printf("Inputs\n");


		/************************Climber Functions************************/

		// PTO
		if (m_operator_stick->IsButtonClicked(
				DriverStationConfig::JoystickButtons::ENGAGE_PTO))
		{
			m_componentData->climberData->enableDebug();
			//			m_componentData->shooterData->SetEnabled(false);
			m_componentData->climberData->EngagePTO();
		}
		else if (m_operator_stick->IsButtonClicked(
				DriverStationConfig::JoystickButtons::DISENGAGE_PTO))
		{
			m_componentData->climberData->enableDebug();
			//			m_componentData->shooterData->SetEnabled(false);
			m_componentData->climberData->DisengagePTO();
		}
		else
		{
			m_componentData->climberData->disableDebug();
		}
		// Winch Pawl
		if (m_operator_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::PAWL_UP))
		{
			m_componentData->climberData->enableDebug();
			//			m_componentData->shooterData->SetEnabled(false);
			m_componentData->climberData->winchPawlUp();

		}
		else if (m_operator_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::PAWL_DOWN))
		{
			m_componentData->climberData->enableDebug();
			//			m_componentData->shooterData->SetEnabled(false);
			m_componentData->climberData->winchPawlDown();
		}
		else
		{
			m_componentData->climberData->winchPawlInactive();
			m_componentData->climberData->disableDebug();
		}
		// Climber Arms
		if (m_operator_stick->IsButtonClicked(
				DriverStationConfig::JoystickButtons::CLIMBER_ARMS))
		{
			m_componentData->climberData->enableDebug();
			m_componentData->climberData->changeArmState();
		}
		if (m_componentData->climberData->getCurrentState() != NOTHING)
		{
			m_componentData->climberData->setShouldContinueClimbing(
					m_operator_stick->IsButtonClicked(
							DriverStationConfig::JoystickButtons::CONTINUE_CLIMB));
			if (m_operator_stick->IsButtonClicked(
					DriverStationConfig::JoystickButtons::SHOOTER_ON))
			{
				if (m_operator_stick->IsButtonDown(
						DriverStationConfig::JoystickButtons::CONDITIONAL_ABORT))
				{
					m_componentData->climberData->setShouldPotentiallyAbort(
							true);
				}
				else
				{
					m_componentData->climberData->setShouldPotentiallyAbort(
							false);
				}
			}
		}
		else
		{
			m_componentData->climberData->setShouldHooksDown(
					m_driver_stick->IsButtonClicked(
							DriverStationConfig::JoystickButtons::START_CLIMB));
			//MCCC
			if (m_driver_stick->IsButtonDown(DriverStationConfig::JoystickButtons::START_CLIMB)) {
				holdTicks++;
			} else {
				holdTicks = 0;
			}
			if (holdTicks >= 50) {
				m_componentData->climberData->setShouldContinueClimbing(true);
				m_componentData->climberData->setShouldHooksDown(false);
			}
			//end MCCC
		}

		/************************Shooter************************/
		if (m_driver_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::FIRE))
		{
			if (continuous)
				m_componentData->shooterData->SetShooterSetting(CONTINUOUS);
			else
				m_componentData->shooterData->SetShooterSetting(ONCE);
			lastPressed = true;
			doubleTimeout = 0;
		}
		else if (m_driver_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::MANUAL_FIRE))
		{
			m_componentData->shooterData->SetShooterSetting(MANUAL);
		}
		else
		{
			m_componentData->shooterData->SetShooterSetting(OFF);
			if (lastPressed)
			{
				continuous = true;
				doubleTimeout++;
				if (doubleTimeout >= 20)
				{
					lastPressed = false;
					continuous = false;
					doubleTimeout = 0;
				}
			}
		}

		if (m_componentData->climberData->getCurrentState() == NOTHING)
		{
			if (m_operator_stick->IsButtonClicked(
					DriverStationConfig::JoystickButtons::SHOOTER_ON))
			{
				m_componentData->shooterData->SetEnabled(true);
			}
		}
		
		if (m_operator_stick->IsButtonClicked(
			DriverStationConfig::JoystickButtons::CHANGE_ANGLE))
		{
				if (m_componentData->shooterData->ShouldLauncherBeHigh())
						m_componentData->shooterData->SetLauncherAngleLow();
				else
						m_componentData->shooterData->SetLauncherAngleHigh();
		}
		
		if (m_operator_stick->IsButtonClicked(
			DriverStationConfig::JoystickButtons::SHOOTER_OFF))
		{
			m_componentData->shooterData->SetEnabled(false);
		}

		//		if (fabs(m_driver_stick->GetRawAxis(4)) < 0.1)
		//		{
		//			m_componentData->shooterData->SetSpeedOffset(1.0);
		//		}
		//		else
		//		{
		//			m_componentData->shooterData->SetSpeedOffset(
		//					1 - 0.5 * (m_driver_stick->GetRawAxis(4) - Util::Sign(
		//							m_driver_stick->GetRawAxis(4)) * 0.1) / 0.9);
		//		}

		/************************Automatic Functions************************/

		if (m_driver_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::AUTO_AIM_BACKBOARD))
		{
			m_autoActions->EnableAutoAimBackboard();
		}
		//		else if (m_driver_stick->IsButtonDown(
		//				DriverStationConfig::JoystickButtons::START_CLIMB))
		//		{
		//			m_autoActions->EnableAutoAimPyramid();
		//		}
		//	else if (m_driver_stick->IsButtonDown(
		//			DriverStationConfig::JoystickButtons::FEEDER_STATION_APPROACH))
		//	{
		//		m_autoActions->EnableFeederStationTrack();
		//	}
		else
		{
			m_autoActions->Reset();
		}

		/************************Collector************************/
		if (m_driver_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::COLLECTOR_SLIDE))
		{
			m_componentData->collectorData->SlideDown();
			m_componentData->collectorData->RunRollers();
		}
		else if (m_operator_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::COLLECTOR_DOWN_NO_MOTOR))
		{
			m_componentData->collectorData->SlideDown();
		}
		else
		{
			m_componentData->collectorData->SlideUp();
			m_componentData->collectorData->StopRollers();
		}
		if (m_operator_stick->IsButtonDown(
				DriverStationConfig::JoystickButtons::PURGE))
		{
			m_componentData->collectorData->RunRollersBackwards();
		}
	}
	else
		lastStop = false;
	LCD::Instance()->Print(2, 0, false, "Auto Aim");
	LCD::Instance()->Print(2, 12, false, "o");
	if (m_componentData->autoAimData->getDesiredX()
			- m_componentData->autoAimData->getCurrentX()
			< -m_componentData->autoAimData->getErrorThreshold())
	{
		// we're to the right
		//m_componentData->ledIndicatorData->setColorRGB(0, 0, 127, data::indicators::LEFT_ARROW);
		//m_componentData->ledIndicatorData->setColorRGB(0, 0, 0, data::indicators::RIGHT_ARROW);
		LCD::Instance()->Print(2, 10, false, "<");
		LCD::Instance()->Print(2, 14, false, " ");
	}
	else if (m_componentData->autoAimData->getDesiredX()
			- m_componentData->autoAimData->getCurrentX()
			> m_componentData->autoAimData->getErrorThreshold())
	{
		// we're to the left
		//m_componentData->ledIndicatorData->setColorRGB(127, 0, 0, data::indicators::RIGHT_ARROW);
		//m_componentData->ledIndicatorData->setColorRGB(0, 0, 0, data::indicators::LEFT_ARROW);
		LCD::Instance()->Print(2, 10, false, " ");
		LCD::Instance()->Print(2, 14, false, ">");
	}
	else
	{
		// we're lined up!
		//m_componentData->ledIndicatorData->setColorRGB(0, 127, 0, data::indicators::LEFT_ARROW);
		//m_componentData->ledIndicatorData->setColorRGB(0, 127, 0, data::indicators::RIGHT_ARROW);
		LCD::Instance()->Print(2, 10, false, "<");
		LCD::Instance()->Print(2, 14, false, ">");
	}
	if (m_componentData->autoAimData->getDesiredY()
			- m_componentData->autoAimData->getCurrentY()
			< -m_componentData->autoAimData->getErrorThreshold())
	{
		// we're back
		//		m_componentData->ledIndicatorData->setColorRGB(0, 0, 0, data::indicators::DOWN_ARROW);
		//		m_componentData->ledIndicatorData->setColorRGB(0, 0, 127, data::indicators::UP_ARROW);
		LCD::Instance()->Print(3, 20, false, "^");
		LCD::Instance()->Print(1, 20, false, " ");
	}
	else if (m_componentData->autoAimData->getDesiredX()
			- m_componentData->autoAimData->getCurrentX()
			> m_componentData->autoAimData->getErrorThreshold())
	{
		// we're forward
		//		m_componentData->ledIndicatorData->setColorRGB(127, 0, 0, data::indicators::DOWN_ARROW);
		//		m_componentData->ledIndicatorData->setColorRGB(0, 0, 0, data::indicators::UP_ARROW);
		LCD::Instance()->Print(3, 20, false, " ");
		LCD::Instance()->Print(1, 20, false, "v");
	}
	else
	{
		// we're lined up!
		//		m_componentData->ledIndicatorData->setColorRGB(0, 127, 0, data::indicators::DOWN_ARROW);
		//		m_componentData->ledIndicatorData->setColorRGB(0, 127, 0, data::indicators::UP_ARROW);
		LCD::Instance()->Print(3, 20, false, "v");
		LCD::Instance()->Print(1, 20, false, "^");
	}

	/************************Config************************/

	if (m_driver_stick->IsButtonClicked(
			DriverStationConfig::JoystickButtons::SAVE_CONFIG))
		m_componentData->configLoaderData->RequestSave();
	if (m_driver_stick->IsButtonClicked(
			DriverStationConfig::JoystickButtons::LOAD_CONFIG))
		m_componentData->configLoaderData->RequestLoad();
	if (m_driver_stick->IsButtonClicked(
			DriverStationConfig::JoystickButtons::APPLY_CONFIG))
		m_componentData->configLoaderData->RequestApply();

	/************************Routine Recorder************************/
	//	if (m_operator_stick->IsButtonDown(
	//			DriverStationConfig::JoystickButtons::RECORD)
	//			&& !m_operator_stick->IsButtonDown(
	//					DriverStationConfig::JoystickButtons::SHOOTER_OFF))
	//	{
	//		m_componentData->routineRecorderData->ShouldRecord(true);
	//	}
	//	else
	//	{
	//		m_componentData->routineRecorderData->ShouldRecord(false);
	//	}
}
