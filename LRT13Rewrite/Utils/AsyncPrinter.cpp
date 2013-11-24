#include "AsyncPrinter.h"

AsyncPrinter *AsyncPrinter::_instance = NULL;

void AsyncPrinter::Initialize()
{
	if(!_instance)
	{
		_instance = new AsyncPrinter();
	}
	
	_instance->Start();
}

void AsyncPrinter::Finalize()
{
	_instance->Abort(0, 1.0 / 50);
	delete _instance;
}

AsyncPrinter::AsyncPrinter()
	: AsyncProcess("AsyncPrinter", Task::kDefaultPriority + 1)
{
	m_queueSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	m_ioSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
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
		Synchronized s(_instance->m_queueSem);

		string str(buffer);

		_instance->_messageQueue.push(str);
		//me.queue_bytes_ += n_bytes;

		if (_instance->_messageQueue.size() >= kMaxQueueSize)
		{
			while (!_instance->_messageQueue.empty())
				_instance->_messageQueue.pop();

			string overflow("(AsyncPrinter Buffer Overflow)\n");

			_instance->_messageQueue.push(overflow);
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

void AsyncPrinter::RedirectToFile(const char* file)
{
	Synchronized s(_instance->m_ioSem);
	fflush(_instance->m_out);
	if(_instance->m_out != stdout) // don't want to close stdout
		fclose(_instance->m_out);
	_instance->m_out = fopen(file, "w");
}

void AsyncPrinter::RestoreToConsole()
{
	Synchronized s(_instance->m_ioSem);
	fflush(_instance->m_out);
	if(_instance->m_out != stdout) // don't want to close stdout
		fclose(_instance->m_out);
	_instance->m_out = stdout;
}

INT32 AsyncPrinter::Tick()
{
	UINT32 printed = 0;
	
	Synchronized s(m_queueSem);
	Synchronized s2(m_ioSem);
	
	while(!_messageQueue.empty() && ++printed <= kMaxPrintsPerCycle && IsRunning())
	{
		string msg = _messageQueue.front();
		_messageQueue.pop();
		
		if(m_out != NULL)
			fprintf(m_out, msg.c_str());
	}
	
	taskDelay(sysClkRateGet() / 1000);
	
	return 0;
}
