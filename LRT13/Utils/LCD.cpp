#include "LCD.h"
#include "../Utils/Util.h"
#include "../Utils/AsyncPrinter.h"
#include "NetworkCommunication/FRCComm.h"
#include <cstdio>
#include <Synchronized.h>

LCD* LCD::instance = NULL;
double LCD::gameTime = -1.0;

LCD::LCD() :
	SynchronizedProcess("LCD"),
	loadArray("\\|/-")
{
	textBuffer = new char[kNumBufferLines * kNumBufferColumns];
	outputBuffer = new char[USER_DS_LCD_DATA_SIZE];

	curLineIndex = curColumnIndex = 0;
	memset(textBuffer, ' ', kNumBufferLines * kNumBufferColumns);

	textBufferSemaphore = semMCreate(SEM_DELETE_SAFE | SEM_INVERSION_SAFE);
}

LCD::~LCD()
{
	semDelete(textBufferSemaphore);
	delete[] textBuffer;
	delete[] outputBuffer;
}

LCD* LCD::Instance()
{
	if (instance == NULL)
		instance = new LCD();
	return instance;
}

void LCD::Finalize()
{
	DELETE(instance);
}

void LCD::Print(UINT8 line, UINT8 index, const char* format, ...)
{
	if (line > kNumBufferLines)
	{
		AsyncPrinter::Printf("LCD: out of bounds %d\n", line);
		return;
	}

	va_list args;

	char buffer[120]; // the largest possible string that would be passed
	int len;

	va_start(args, format);

	Synchronized sync(textBufferSemaphore);
	len = vsprintf(buffer, format, args);

	va_end(args);

//	memset(textBuffer + line * kNumBufferColumns + index, ' ', kNumBufferColumns);

	// limit the maximum length to write to the line
	if (len > kNumBufferColumns - index)
		len = kNumBufferColumns - index;

	memcpy(textBuffer + line * kNumBufferColumns + index, buffer, len);
}

void LCD::LCDUpdate()
{
	Synchronized sync(textBufferSemaphore);

	// clear the buffer
	memset(outputBuffer, ' ', USER_DS_LCD_DATA_SIZE);
	*((UINT16*) outputBuffer) = kFullDisplayTextCommand;

	char* outputBufferTextStart = outputBuffer + sizeof(UINT16);

	for (int line = 0; line < kNumLcdPhysicalLines; ++line)
	{
		int y = curLineIndex + line;
		memcpy(outputBufferTextStart + kNumLcdPhysicalColumns * line,
				textBuffer + kNumBufferColumns * y + curColumnIndex,
				kNumLcdPhysicalColumns);
	}

	setUserDsLcdData(outputBuffer, USER_DS_LCD_DATA_SIZE, kSyncTimeout_ms);
}

void LCD::ScrollLCD(int x, int y)
{
	curLineIndex = Util::Clamp<int>(curLineIndex + y, 0,
			kNumBufferLines - kNumLcdPhysicalLines);
	curColumnIndex = Util::Clamp<int>(curColumnIndex + x, 0,
			kNumBufferColumns - kNumLcdPhysicalColumns);

	AsyncPrinter::Printf("DS LCD Scroll %d %d => %d,%d\n", x, y, curLineIndex,
			curColumnIndex);
}

INT32 LCD::Tick()
{
	static int loops = 0;

	if (loops % 5 == 0)
		LCDUpdate();

	char heartbeat = loadArray[(loops / 10) % 4];
	Print(kHeartbeatLine, 0, "%c  %s  %.2f", heartbeat, "Ultimate Funky Object", gameTime);

	loops++;
	
	return 0;
}

void LCD::UpdateGameTime(double time)
{
	gameTime = time;
}
