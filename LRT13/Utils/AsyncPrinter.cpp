#include "AsyncPrinter.h"

AsyncPrinter* AsyncPrinter::_instance = NULL;

AsyncPrinter* AsyncPrinter::Instance()
{
	if(!_instance)
	{
		_instance = new AsyncPrinter();
		_instance->Start();
	}
	
	return _instance;
}

void AsyncPrinter::Finalize()
{
	DELETE(_instance);
}

AsyncPrinter::AsyncPrinter()
	: AsyncProcess("AsyncPrinter", Task::kDefaultPriority + 1)
{
	m_queueSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
}

AsyncPrinter::~AsyncPrinter()
{
	semDelete(m_queueSem);
	
	m_queueSem = NULL;
}

int AsyncPrinter::Printf(const char * msg, ...)
{
	char buffer[256];
	
	va_list args;
	va_start(args, msg);

	int n_bytes = vsprintf(buffer, msg, args);
	va_end(args);
	
	if (n_bytes >= 0)
	{
		Synchronized s(Instance()->m_queueSem);

		string str(buffer);

		Instance()->_messageQueue.push(str);
		//me.queue_bytes_ += n_bytes;

		if (Instance()->_messageQueue.size() >= kMaxQueueSize)
		{
			while (!Instance()->_messageQueue.empty())
				Instance()->_messageQueue.pop();

			string overflow("(AsyncPrinter Buffer Overflow)\n");

			Instance()->_messageQueue.push(overflow);
		}
	}
	
	return n_bytes;
}

int AsyncPrinter::Println(const char * msg, ...)
{
	va_list args;
	va_start(args, msg);
	
	char res[256];
	strcpy(res, msg);
	strcat(res, "\n");
	
	int returned = AsyncPrinter::Printf(res, args);
	
	va_end(args);
	
	return returned;
}

void AsyncPrinter::DbgPrint(const char* msg, ...)
{
#ifdef DEBUG
	va_list args;
	va_start(args, msg);
	
	char res[256];
	strcpy(res, msg);
	strcat(res, "\n");
		
	printf(res, args);
	
	va_end(args);
#endif
}

INT32 AsyncPrinter::Tick()
{
	UINT32 printed = 0;
	
	{
		Synchronized s(m_queueSem);
		
		while(!_messageQueue.empty() && ++printed <= kMaxPrintsPerCycle && IsRunning())
		{
			string msg = _messageQueue.front();
			_messageQueue.pop();
			
			printf(msg.c_str());
		}
	}
	
	Wait(0.001);
	
	return 0;
}
