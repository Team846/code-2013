#ifndef ROUTINE_RECORDER_H_
#define ROUTINE_RECORDER_H_

#include "Component.h"
#include "../ComponentData/RoutineRecorderData.h"
#include "../ComponentData/DrivetrainData.h"
#include "../ComponentData/CollectorData.h"
#include "../ComponentData/ShooterData.h"
#include "../Sensors/DriveEncoders.h"

/*!
 * @brief Records driving
 * @author Raphael Chang
 */
class RoutineRecorder : public Component
{
public:
	RoutineRecorder();
	~RoutineRecorder();
	
	virtual void onEnable(); 
	virtual void onDisable();
	
	virtual void enabledPeriodic();
	virtual void disabledPeriodic();
	
	typedef struct Step
	{
		double forward;
		double turn;
	};
	
	typedef struct Cycle
	{
		double forward;
		double turn;
		bool collecting;
		bool shooting;
		bool angleHigh;
	};
private:
	DriveEncoders* m_driveEncoders;
	queue<Cycle> m_routine;
	bool m_lastRecording;
	ofstream m_file;
};

#endif
