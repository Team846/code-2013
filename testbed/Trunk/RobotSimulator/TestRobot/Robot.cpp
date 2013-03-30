#include <cmath>

#include "Utils/AsyncPrinter.h"

#include "Networking\NetServer.h"
#include "Networking\MessageType.h"

int ThreadEntry(UINT32 code)
{
	while(true)
	{
		printf("%c!\n", code);

		Sleep(15);
	}
}

class Robot : public RobotBase
{
public:
	Robot();
	virtual ~Robot();

	void StartCompetition();
	void RobotInit();
	void MainLoop();
};

Robot::Robot()
{
	sockaddr s;
}

Robot::~Robot()
{
	AsyncPrinter::Instance()->Abort();
}

void Robot::StartCompetition()
{
	NetServer* server = new NetServer(846);

	server->Open();

	printf("Robot::StartCompetition() task name: %s\n", m_task->GetName());

	//for(int i = 0; i < 100; i++)

	Sleep(1000);

	printf("\n");

	float totalTime = 0;

	int i = 0;
	while(true)
	{
		printf("%d\n", i++);
		DriverStationInput::NewData();

		NetBuffer buff;

		float value = fabs(sin((float)totalTime / 1000.0f));

		buff.Write((char)MessageType::FRONT_SHOOTER_DATA);
		buff.Write((float)totalTime / 1000.0f);
		buff.Write(value);

		server->SendToAll(&buff, NetChannel::NET_UNRELIABLE_SEQUENCED, 1);

		printf("Sending value of %f at time %f", value, (float)totalTime / 1000.0f); 

		Sleep(10);

		totalTime += 10;

		printf("\n");
	}
	
}

RobotBase* FRC_userClassFactory()
{
	return new Robot();
}

extern "C"
{
__declspec(dllexport) INT32 FRC_UserProgram_StartupLibraryInit()
{
	RobotBase::startRobotTask((FUNCPTR) FRC_userClassFactory);
	return 0;
}
}

extern "C" __declspec(dllexport) void InteropTest()
{
	printf("From unmanaged DLL!\n");
}

int main()
{
	printf("RobotSimulator 1.0\n");
	printf("\n");

	//RBase *robot = new Rbt();

	//RobotBase* robot = new Robot();

	//printf("!");

	//FRC_userClassFactory();

	RobotBase::startRobotTask((FUNCPTR)FRC_userClassFactory);
	/*
	Task *t = new Task("test task", (FUNCPTR)ThreadEntry, 101, 20000);
	t->Start('a');

	Task *t2 = new Task("test task 2", (FUNCPTR)ThreadEntry, 101, 20000);
	t->Start('b');*/

	while(true) ;
	//system("PAUSE");
}
