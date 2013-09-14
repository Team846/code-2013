#include "AutonomousRoutines.h"
#include "../ComponentData/ComponentData.h"
#include "../ComponentData/CollectorData.h"
#include "../ComponentData/ShooterData.h"
#include "AutoActions.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"
#include <algorithm>

#include "Routines/Drive.h"
#include "Routines/Turn.h"
#include "Routines/Collect.h"
#include "Routines/Shoot.h"
#include "Routines/Angle.h"
#include "Routines/Pause.h"
#include "Routines/RoutineGroup.h"

using namespace data;
using namespace data::drivetrain;

AutonomousRoutines::AutonomousRoutines()
{
	m_componentData = ComponentData::GetInstance();

	m_autoActions = AutoActions::Instance();

	m_autonomousStartTime = 0.0;

	beginNext = false;
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
	if (RobotData::GetCurrentState() == RobotData::AUTONOMOUS)
	{
		if (RobotData::GetLastState() != RobotData::GetCurrentState())
		{
			int autonRoutine = (int)(DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_SELECT) + 0.5) + 1;
			AsyncPrinter::Printf("Starting autonomous routine %d\n", autonRoutine);
			m_autonomousStartTime = Timer::GetFPGATimestamp();

			while (!routines.empty())
				routines.pop();
			
			LoadRoutine(RobotConfig::ROUTINE_FILE_PATH + Util::lexical_cast(autonRoutine));

			beginNext = true;
			
			m_componentData->shooterData->SetEnabled(true);
		}
		if (!routines.empty())
		{
			if (beginNext)
			{
				printf("Next routine\n");
				routines.front()->Run();
				beginNext = false;
			}
			if (routines.front()->Completed())
			{
				routines.front()->Stop();
				printf("Routine finished\n");
				delete routines.front();
				printf("Routine deleted\n");
				routines.pop();
				beginNext = true;
			}
		}
	}

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
	ifstream fin(path.c_str());
	if (!fin.is_open())
	{
		AsyncPrinter::Printf("Cannot open autonomous routine file: %s\n",
				path.c_str());
		return;
	}

	int lineNumber = 1;
	while (!fin.eof())
	{
		// Going through the file line by line for each command
		string line;
		getline(fin, line);
		stringstream nocomments(line);
		getline(nocomments, line, '#');
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); // Remove spaces
		if (line.length() == 0)
			continue;
		vector<string> routineList;
		unsigned int pos;
		while ((pos = line.find("),")) != std::string::npos) // Look for parallel commands
		{
			routineList.push_back(line.substr(0, pos + 1));
			line.erase(0, pos + 2);
		}
		routineList.push_back(line);
		vector<Routine*> parallelRoutines;
		for (vector<string>::iterator it = routineList.begin(); it
				< routineList.end(); it++)
		{
			Routine *current;
			stringstream sstream(*it);
			string command, args;
			getline(sstream, command, '('); // Get command name

			getline(sstream, args, ')');

			stringstream argstream(args);
			string temp;
			vector<string> arglist;

			// Check for commas delimiting args
			while (getline(argstream, temp, ','))
			{
				arglist.push_back(temp);
			}

			AsyncPrinter::Printf("Command %s\n", command.c_str());
			bool failed = false;

			if (command == "drive")
			{
				if (arglist.size() == 1)
					current = new Drive(Util::lexical_cast<double>(arglist[0]));
				else if (arglist.size() == 2)
					current = new Drive(Util::lexical_cast<double>(arglist[0]),
							Util::lexical_cast<double>(arglist[1]));
				else if (arglist.size() == 3)
					current = new Drive(Util::lexical_cast<double>(arglist[0]),
							Util::lexical_cast<double>(arglist[1]),
							Util::lexical_cast<double>(arglist[2]));
				else
					failed = true;
			}
			else if (command == "turn")
			{
				if (arglist.size() == 1)
					current = new Turn(Util::lexical_cast<double>(arglist[0]));
				else if (arglist.size() == 2)
					current = new Turn(Util::lexical_cast<double>(arglist[0]),
							Util::lexical_cast<double>(arglist[1]));
				else if (arglist.size() == 3)
					current = new Turn(Util::lexical_cast<double>(arglist[0]),
							Util::lexical_cast<double>(arglist[1]),
							Util::lexical_cast<double>(arglist[2]));
				else
					failed = true;
			}
			else if (command == "shoot")
			{
				if (arglist.size() == 1)
					current = new Shoot(Util::lexical_cast<int>(arglist[0]));
				else if (arglist.size() == 2)
					current = new Shoot(Util::lexical_cast<int>(arglist[0]),
							Util::lexical_cast<double>(arglist[1]));
				else
					failed = true;
			}
			else if (command == "collect")
			{
				if (arglist.size() == 1)
					current = new Collect(Util::lexical_cast<bool>(arglist[0]));
				else
					failed = true;
			}
			else if (command == "angle")
			{
				if (arglist.size() == 1)
					current = new Angle(Util::lexical_cast<bool>(arglist[0]));
				else
					failed = true;
			}
			else if (command == "wait")
			{
				if (arglist.size() == 1)
					current = new Pause(Util::lexical_cast<double>(arglist[0]));
				else
					failed = true;
			}
			else
			{
				AsyncPrinter::Printf(
						"[WARNING] Unknown routine: %s on line %d, ignoring.\n",
						command.c_str(), lineNumber);
				continue;
			}
			if (failed)
			{
				AsyncPrinter::Printf(
						"[WARNING] Incorrect number of arguments for routine: %s on line %d, ignoring.\n",
						command.c_str(), lineNumber);
				continue;
			}
			parallelRoutines.push_back(current);
		}
		if (parallelRoutines.size() > 1)
		{
			routines.push(new RoutineGroup(parallelRoutines));
		}
		else
		{
			routines.push(parallelRoutines[0]);
		}
		lineNumber++;
	}
	AsyncPrinter::Printf("Done loading autonomous routine file: %s\n",
			path.c_str());
	fin.close();
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
