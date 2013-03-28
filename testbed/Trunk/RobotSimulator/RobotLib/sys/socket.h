#ifndef SYS_SOCKET_H_
#define SYS_SOCKET_H_

#pragma comment(lib, "ws2_32.lib") // load winsock2 library
#include <WinSock2.h>
#include <Windows.h>
#include <ctime>

int close(int socket);

#endif