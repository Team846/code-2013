#include "socket.h"

int close(int socket)
{
	return closesocket(socket);
}