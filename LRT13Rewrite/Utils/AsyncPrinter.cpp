#include "AsyncPrinter.h"

AsyncPrinter *AsyncPrinter::_instance = NULL;
bool AsyncPrinter::filePrinting = false;
int AsyncPrinter::fd;
fpos_t AsyncPrinter::pos;

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
	if (!filePrinting)
	{
		Synchronized s(_instance->m_queueSem);
		printf("\n--------------------Redirecting print stream to file--------------------\n");
		fflush(stdout);
		fgetpos(stdout, &pos);
		fd = dup(fileno(stdout));
		freopen(file, "a", stdout);
		printf("\n--------------------Print stream redirected--------------------\n");
		filePrinting = true;
	}
}

void AsyncPrinter::RestoreToConsole()
{
	if (filePrinting)
	{
		Synchronized s(_instance->m_queueSem);
		printf("\n--------------------Restoring print stream to console--------------------\n");
		fflush(stdout);
		dup2(fd, fileno(stdout));
		close(fd);
		clearerr(stdout);
		fsetpos(stdout, &pos); 
		printf("\n--------------------Print stream restored--------------------\n");
		filePrinting = false;
	}
}

void AsyncPrinter::Tick()
{
	UINT32 printed = 0;
	
	Synchronized s(m_queueSem);
	
	while(!_messageQueue.empty() && ++printed <= kMaxPrintsPerCycle && IsRunning())
	{
		string msg = _messageQueue.front();
		_messageQueue.pop();
		
		printf(msg.c_str());
	}
	
	taskDelay(sysClkRateGet() / 1000);
}
