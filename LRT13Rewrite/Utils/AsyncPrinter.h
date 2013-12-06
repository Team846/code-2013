#ifndef ASYNC_PRINTER_H_
#define ASYNC_PRINTER_H_

#include <WPILib.h>
#include <queue>
#include <string>

#include "../Process/AsyncProcess.h"

/*!
 * @brief Asynchronous process for non-blocking printf.
 */

class AsyncPrinter : public AsyncProcess
{
public:
	static void Initialize();
	static void Finalize();
	
	static int Printf(const char* msg, ...);
	static int Println(const char* msg, ...);
	static void DbgPrint(const char* msg, ...);
	
	static void RedirectToFile(const char* file);
	static void RestoreToConsole();
	
	AsyncPrinter();
	~AsyncPrinter();
	
	static const UINT32 kMaxQueueSize = 4096;
	static const UINT32 kMaxPrintsPerCycle = 50;
	
protected:
	void Tick();
	
private:
	static AsyncPrinter* _instance;
	
	SEM_ID m_queueSem;
	
	queue<string> _messageQueue;
	
	static bool filePrinting;
	static int fd;
	static fpos_t pos;
};

#endif
