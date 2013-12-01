#include "Autonomous.h"
#include "../Config/RobotConfig.h"
#include "../Config/DriverStationConfig.h"
#include "../Utils/AsyncPrinter.h"
#include "../Utils/Util.h"
#include <algorithm>

//#include "Routines/Drive.h"
//#include "Routines/Turn.h"
//#include "Routines/Collect.h"
//#include "Routines/Shoot.h"
//#include "Routines/Angle.h"
//#include "Routines/Arms.h"
//#include "Routines/Arc.h"
//#include "Routines/Pause.h"
//#include "Routines/RoutineGroup.h"

Autonomous::Autonomous()
{
//	m_componentData = ComponentData::GetInstance();

	m_autonomousStartTime = 0.0;

	beginNext = false;
}

Autonomous::~Autonomous()
{

}

Automation::Status Autonomous::Start(Event *trigger)
{
	int autonRoutine = (int)(DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_SELECT) + 0.5) + 1;
	AsyncPrinter::Printf("Starting autonomous routine %d\n", autonRoutine);
	m_autonomousStartTime = Timer::GetFPGATimestamp();
	
	while (!routines.empty())
		routines.pop();
	
	double delay = DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_DELAY);
	
	if (delay > 0.0)
	{
//				routines.push(new Pause(delay));
	}
	
	LoadRoutine(RobotConfig::ROUTINE_FILE_PATH.substr(0, RobotConfig::ROUTINE_FILE_PATH.find('.')) + Util::lexical_cast(autonRoutine) + RobotConfig::ROUTINE_FILE_PATH.substr(RobotConfig::ROUTINE_FILE_PATH.find('.'), RobotConfig::ROUTINE_FILE_PATH.length() - RobotConfig::ROUTINE_FILE_PATH.find('.')));

	beginNext = true;
	
//			m_componentData->drivetrainData->zeroLastPositionSetpoint(FORWARD);
//			m_componentData->drivetrainData->zeroLastPositionSetpoint(TURN);
//			m_componentData->shooterData->SetEnabled(true);
	return SUCCESS;
}

bool Autonomous::Run()
{
	if (routines.empty())
		return true;
	if (routines.front()->Completed())
	{
		routines.front()->Stop();
		delete routines.front();
		routines.pop();
		beginNext = true;
	}
	if (beginNext)
	{
		routines.front()->Run();
		beginNext = false;
	}
	return false;
}

Automation::Status Autonomous::Abort(Event *trigger)
{
	while (!routines.empty())
		routines.pop();
	return SUCCESS;
}

void Autonomous::AllocateResources()
{
	
}

void Autonomous::LoadRoutine(std::string path)
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

//			if (command == "drive")
//			{
//				if (arglist.size() == 1)
//					current = new Drive(Util::lexical_cast<double>(arglist[0]));
//				else if (arglist.size() == 2)
//					current = new Drive(Util::lexical_cast<double>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]));
//				else if (arglist.size() == 3)
//					current = new Drive(Util::lexical_cast<double>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]),
//							Util::lexical_cast<double>(arglist[2]));
//				else if (arglist.size() == 4)
//					current = new Drive(Util::lexical_cast<double>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]),
//							Util::lexical_cast<double>(arglist[2]),
//							Util::lexical_cast<bool>(arglist[3]));
//				else
//					failed = true;
//			}
//			else if (command == "turn")
//			{
//				if (arglist.size() == 1)
//					current = new Turn(Util::lexical_cast<double>(arglist[0]));
//				else if (arglist.size() == 2)
//					current = new Turn(Util::lexical_cast<double>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]));
//				else if (arglist.size() == 3)
//					current = new Turn(Util::lexical_cast<double>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]),
//							Util::lexical_cast<double>(arglist[2]));
//				else
//					failed = true;
//			}
//			else if (command == "arc")
//			{
//				if (arglist.size() == 2)
//					current = new Arc(Util::lexical_cast<double>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]));
//				else if (arglist.size() == 3)
//					current = new Arc(Util::lexical_cast<double>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]),
//							Util::lexical_cast<double>(arglist[2]));
//				else if (arglist.size() == 4)
//					current = new Arc(Util::lexical_cast<double>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]),
//							Util::lexical_cast<double>(arglist[2]),
//							Util::lexical_cast<double>(arglist[3]));
//				else if (arglist.size() == 5)
//					current = new Arc(Util::lexical_cast<double>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]),
//							Util::lexical_cast<double>(arglist[2]),
//							Util::lexical_cast<double>(arglist[3]),
//							Util::lexical_cast<double>(arglist[4]));
//				else
//					failed = true;
//			}
//			else if (command == "shoot")
//			{
//				if (arglist.size() == 1)
//					current = new Shoot(Util::lexical_cast<int>(arglist[0]));
//				else if (arglist.size() == 2)
//					current = new Shoot(Util::lexical_cast<int>(arglist[0]),
//							Util::lexical_cast<double>(arglist[1]));
//				else
//					failed = true;
//			}
//			else if (command == "collect")
//			{
//				if (arglist.size() == 1)
//					current = new Collect(Util::lexical_cast<bool>(arglist[0]));
//				else
//					failed = true;
//			}
//			else if (command == "angle")
//			{
//				if (arglist.size() == 1)
//					current = new Angle(Util::lexical_cast<bool>(arglist[0]));
//				else
//					failed = true;
//			}
//			else if (command == "arms")
//			{
//				if (arglist.size() == 1)
//					current = new Arms(Util::lexical_cast<bool>(arglist[0]));
//				else
//					failed = true;
//			}
//			else if (command == "wait")
//			{
//				if (arglist.size() == 1)
//					current = new Pause(Util::lexical_cast<double>(arglist[0]));
//				else
//					failed = true;
//			}
//			else
//			{
//				AsyncPrinter::Printf(
//						"[WARNING] Unknown routine: %s on line %d, ignoring.\n",
//						command.c_str(), lineNumber);
//				continue;
//			}
//			if (failed)
//			{
//				AsyncPrinter::Printf(
//						"[WARNING] Incorrect number of arguments for routine: %s on line %d, ignoring.\n",
//						command.c_str(), lineNumber);
//				continue;
//			}
//			parallelRoutines.push_back(current);
		}
		if (parallelRoutines.size() > 1)
		{
//			routines.push(new RoutineGroup(parallelRoutines));
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

