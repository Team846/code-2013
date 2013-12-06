#ifndef LCD_H_
#define LCD_H_

#include <WPILib.h>
#include "../Process/SynchronizedProcess.h"
#include "../RobotState.h"

/**
 * Provides LCD output on the Driver Station LCD. Utilizes scrolling.
 */

class LCD : public SynchronizedProcess
{
public:
	static const UINT32 kSyncTimeout_ms = 20;
	static const UINT16 kFullDisplayTextCommand = 0x9FFF;

	enum LCDLines
	{
		kHeartbeatLine = 0,
		kDriveLine = 1,
		kDriveTrainLine = 2,
		kShooterLine = 3,
		kCollectorLine = 4,
		kClimberLine = 5,
		kWinchLine = 6,
		kENDLINES = 7
	};
	
	~LCD();
	static LCD* Instance();
	static void Finalize();

	void LCDUpdate();
	void Print(UINT8 line, UINT8 index, bool clear, const char* format, ...);

	void ScrollLCD(int x, int y);

protected:
	void Tick();

private:
	LCD();
	static LCD *instance;
	DISALLOW_COPY_AND_ASSIGN(LCD);

	int curLineIndex;
	int curColumnIndex;

	static const UINT8 kNumBufferLines = 20;
	static const UINT8 kNumBufferColumns = 40;

	static const UINT8 kNumLcdPhysicalLines = 6;
	// even on the new DriverStation, still 21 char
	static const UINT8 kNumLcdPhysicalColumns = 21;

	const char *loadArray;
	char *textBuffer;
	char *outputBuffer;
	semaphore *textBufferSemaphore;

	RobotState &m_robotState;
};

#endif //LRT_DRIVER_STATION_LCD_H_
