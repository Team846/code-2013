#ifndef ASYNC_PRINTER_H_
#define ASYNC_PRINTER_H_

#include "../Process/AsyncProcess.h"

#include "WPILib.h"
#include <queue>
#include <string>

/*!
 * @brief Asynchronous process for non-blocking printf
 * @author Tony Peng
 */

class AsyncPrinter : public AsyncProcess
{
public:
	static AsyncPrinter* Instance();
	static void Initialize();
	static void Finalize();
	
	static int Printf(const char * msg, ...);
	static int Println(const char * msg, ...);
	
	AsyncPrinter();
	~AsyncPrinter();
	
	static const UINT32 kMaxQueueSize = 4096;
	static const UINT32 kMaxPrintsPerCycle = 50;
	
protected:
	INT32 Tick();
	
private:
	static AsyncPrinter* _instance;
	
	SEM_ID m_queueSem;
	
	queue<string> _messageQueue;
};

#endif
