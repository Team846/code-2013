#include "Autonomous.h"
#include "../../Config/RobotConfig.h"
#include "../../Config/DriverStationConfig.h"
#include "../../Utils/AsyncPrinter.h"
#include "../../Utils/Util.h"
#include <algorithm>

#include "Drive.h"
#include "Turn.h"
#include "CollectorSlide.h"
//#include "Routines/Shoot.h"
//#include "Routines/Angle.h"
//#include "Routines/Arms.h"
//#include "Routines/Arc.h"
#include "Pause.h"
#include "Repeating.h"
#include "Parallel.h"

Autonomous::Autonomous() :
	Sequential("Autonomous")
{
	m_autonomousStartTime = 0.0;
}

Autonomous::~Autonomous()
{

}

bool Autonomous::Start()
{
	int autonRoutine = (int)(DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_SELECT) + 0.5) + 1;
	AsyncPrinter::Printf("Starting autonomous routine %d\n", autonRoutine);
	m_autonomousStartTime = Timer::GetFPGATimestamp();
	
	ClearSequence();
	
	double delay = DriverStation::GetInstance()->GetAnalogIn(DriverStationConfig::AnalogIns::AUTONOMOUS_DELAY);
	
	if (delay > 0.0)
	{
		AddAutomation(new Pause(delay));
	}
	
	LoadRoutine(RobotConfig::ROUTINE_FILE_PATH.substr(0, RobotConfig::ROUTINE_FILE_PATH.find('.')) + Util::lexical_cast(autonRoutine) + RobotConfig::ROUTINE_FILE_PATH.substr(RobotConfig::ROUTINE_FILE_PATH.find('.'), RobotConfig::ROUTINE_FILE_PATH.length() - RobotConfig::ROUTINE_FILE_PATH.find('.')));

	return Sequential::Start();
}

void Autonomous::AllocateResources()
{
	AllocateResource(DRIVE);
	AllocateResource(TURN);
	AllocateResource(COLLECTOR_SLIDE);
	AllocateResource(COLLECTOR_ROLLERS);
	AllocateResource(ANGLE);
}

void Autonomous::LoadRoutine(std::string path)
{
	ifstream fin(path.c_str());
	if (!fin.is_open())
	{
		AsyncPrinter::Printf("Cannot open autonomous routine file: %s\n", path.c_str());
		return;
	}

	int lineNumber = 1;
	while (!fin.eof())
	{
		// Go through the file line by line for each command
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
		vector<Automation*> parallelRoutines;
		for (vector<string>::iterator it = routineList.begin(); it < routineList.end(); it++)
		{
			Automation *current;
			stringstream sstream(*it);
			string command, args;
			getline(sstream, command, '('); // Get command name

			getline(sstream, args, ')');

			stringstream argstream(args);
			string temp;
			vector<string> arglist;

			// Check for commas delimiting arguments
			while (getline(argstream, temp, ','))
			{
				arglist.push_back(temp);
			}

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
				else if (arglist.size() == 4)
					current = new Drive(Util::lexical_cast<double>(arglist[0]),
							Util::lexical_cast<double>(arglist[1]),
							Util::lexical_cast<double>(arglist[2]),
							Util::lexical_cast<bool>(arglist[3]));
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
			else if (command == "collect")
			{
				if (arglist.size() == 1)
					current = new CollectorSlide(Util::lexical_cast<bool>(arglist[0]));
				else
					failed = true;
			}
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
			AddAutomation(new Parallel("AutonomousParallel", parallelRoutines));
		}
		else if (parallelRoutines.size() == 1)
		{
			AddAutomation(parallelRoutines[0]);
		}
		lineNumber++;
	}
	AsyncPrinter::Printf("Done loading autonomous routine file: %s\n", path.c_str());
	fin.close();
}

