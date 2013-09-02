#include "AutonomousRoutines.h"
#include "../ComponentData/ComponentData.h"
#include "../ComponentData/CollectorData.h"
#include "../ComponentData/ShooterData.h"
#include "AutoActions.h"
#include "../Config/RobotConfig.h"
#include "../Utils/AsyncPrinter.h"

#include "Routines/Drive.h"
#include "Routines/Turn.h"

using namespace data;
using namespace data::drivetrain;

AutonomousRoutines::AutonomousRoutines()
{
	m_componentData = ComponentData::GetInstance();

	m_autoActions = AutoActions::Instance();

	m_autonomousStartTime = 0.0;

	beginNext = false;
	
	m_currentState = RobotData::DISABLED;
	m_lastState = RobotData::DISABLED;
}

AutonomousRoutines::~AutonomousRoutines()
{

}

void AutonomousRoutines::TeleopTick()
{
	switch (m_autoActions->GetCurrentAutonomousAction())
	{
	case AutoActions::AUTO_AIM_BACKBOARD:
		ServiceAutoAimBackBoard();
		break;
	case AutoActions::AUTO_AIM_PYRAMID:
		ServiceAutoAimPyramid();
		break;
	case AutoActions::FEEDER_STATION_APPROACH:
		ServiceFeederStationApproach();
		break;
	case AutoActions::NO_ADV_AUTOMATION:
		break;
	}
}

void AutonomousRoutines::Update()
{
	m_currentState = RobotData::GetCurrentState();
	
	if (m_currentState == RobotData::AUTONOMOUS)
	{
		if (m_lastState != m_currentState)
		{
			m_autonomousStartTime = Timer::GetFPGATimestamp();
			
			while (!routines.empty())
				routines.pop();
	
			LoadRoutine(RobotConfig::ROUTINE_FILE_PATH);
			
			beginNext = true;
		}
		if (!routines.empty())
		{
			if (beginNext)
			{
				routines.front()->Run();
				beginNext = false;
			}
			if (routines.front()->Completed())
			{
				delete routines.front();
				routines.pop();
				beginNext = true;
			}
		}
	}
	m_lastState = m_currentState;

	//	fstream fin("/RecordedRoutine.txt");
	//	
	//	if (!fin.is_open())
	//	{
	//		AsyncPrinter::Printf("[ERROR] Could not open recorded autonomous routine file\n");
	//		break;
	//	}
	//	queue<Cycle> routine;
	//	while (!fin.eof())
	//	{
	//		Cycle current;
	//		double forward;
	//		fin >> forward;
	//		current.forward = forward;
	//		double turn;
	//		fin >> current.turn;
	//		current.turn = turn;
	//		fin >> current.collecting;
	//		fin >> current.shooting;
	//		fin >> current.angleHigh;
	//		AsyncPrinter::Printf("next step file %lf\n", current.forward);
	//		routine.push(current);
	//	}
	//	m_componentData->drivetrainData->setControlMode(FORWARD, VELOCITY_CONTROL);
	//	m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
	//	while (!routine.empty())
	//	{
	//		AsyncPrinter::Printf("next step\n");
	//		semTake(loopSem, WAIT_FOREVER);
	//		Cycle current = routine.front();
	//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, current.forward);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, current.turn);
	//		if (current.collecting)
	//		{
	//			m_componentData->collectorData->RunRollers();
	//			m_componentData->collectorData->SlideDown();
	//		}
	//		else
	//		{
	//			m_componentData->collectorData->StopRollers();
	//			m_componentData->collectorData->SlideUp();
	//		}
	//		if (current.shooting)
	//		{
	//			m_componentData->shooterData->SetShooterSetting(CONTINUOUS);
	//		}
	//		else
	//		{
	//			m_componentData->shooterData->SetShooterSetting(OFF);
	//		}
	//		if (current.angleHigh)
	//		{
	//			m_componentData->shooterData->SetLauncherAngleHigh();
	//		}
	//		else
	//		{
	//			m_componentData->shooterData->SetLauncherAngleLow();
	//		}
	//	}
	//	fin.close();
}

void AutonomousRoutines::LoadRoutine(std::string path)
{
	input.open(path.c_str());
	if (!input.is_open())
	{
		AsyncPrinter::Printf("Cannot open autonomous routine file: %s\n", path.c_str());
		return;
	}
	while (!input.eof())
	{
		string line;
		getline(input, line);
		stringstream sstream(line);
		string command;
		getline(sstream, command, '(');
		string param;
		stringstream pstream(param);
		double arg;
		pstream >> arg;
		getline(sstream, command, ')');
		
		if (command == "drive")
			routines.push(new Drive(arg));
		else if (command == "turn")
			routines.push(new Turn(arg));
	}
}

void AutonomousRoutines::ServiceAutoAimBackBoard()
{
	//	if(m_componentData->autoAimData->getDesiredX() - m_componentData->autoAimData->getCurrentX() < -m_componentData->autoAimData->getErrorThreshold())
	//	{
	//		// we're to the right
	//		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, (m_componentData->autoAimData->getDesiredX() - m_componentData->autoAimData->getCurrentX() - m_componentData->autoAimData->getErrorThreshold()) / 100.0);
	//	}
	//	else if(m_componentData->autoAimData->getDesiredX() - m_componentData->autoAimData->getCurrentX() > m_componentData->autoAimData->getErrorThreshold())
	//	{
	//		// we're to the left
	//		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, (m_componentData->autoAimData->getDesiredX() - m_componentData->autoAimData->getCurrentX() + m_componentData->autoAimData->getErrorThreshold()) / 100.0);
	//	}
	//	else
	//	{
	//		// we're lined up!
	//		m_componentData->drivetrainData->setControlMode(TURN, VELOCITY_CONTROL);
	//		m_componentData->drivetrainData->setVelocitySetpoint(TURN, 0);
	//	}
	if (m_componentData->autoAimData->getDesiredY()
			- m_componentData->autoAimData->getCurrentY()
			< -m_componentData->autoAimData->getErrorThreshold())
	{
		// we're too far
		m_componentData->drivetrainData->setControlMode(FORWARD,
				VELOCITY_CONTROL);
		//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, (m_componentData->autoAimData->getDesiredY() - m_componentData->autoAimData->getCurrentY() - m_componentData->autoAimData->getErrorThreshold()) / 100.0);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, -0.2);
	}
	else if (m_componentData->autoAimData->getDesiredY()
			- m_componentData->autoAimData->getCurrentY()
			> m_componentData->autoAimData->getErrorThreshold())
	{
		// we're too close
		m_componentData->drivetrainData->setControlMode(FORWARD,
				VELOCITY_CONTROL);
		//		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, (m_componentData->autoAimData->getDesiredY() - m_componentData->autoAimData->getCurrentY() + m_componentData->autoAimData->getErrorThreshold()) / 100.0);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0.2);
	}
	else
	{
		// we're lined up!
		m_componentData->drivetrainData->setControlMode(FORWARD,
				VELOCITY_CONTROL);
		m_componentData->drivetrainData->setVelocitySetpoint(FORWARD, 0);
	}
}

void AutonomousRoutines::ServiceAutoAimPyramid()
{
	//TODO writeme
}

void AutonomousRoutines::ServiceFeederStationApproach()
{
	//TODO writeme
}
