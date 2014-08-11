// UDP2TCP.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "UDP2TCPProxy.h"
#include <iostream>

#define UDP_SERVER_ADDR "0.0.0.0"
#define UDP_SERVER_PORT 7777
#define TCP_SERVER_ADDR "127.0.0.1"
#define TCP_SERVER_PORT 7777

#define UDP_THREADS	2

using namespace std;

void OnExit(void);	// Invoked while closing an application
CUDP2TCPProxy udp2tcp;

#if defined _WIN32 || defined _WIN64 || __MINGW32__
int	_tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char **argv)
#endif
{
    FSOCK_INIT();		// Initializing sockets
	atexit(OnExit);		// Register exit function
	/////////////////////////////////////////////////

	if (!udp2tcp.Initialize(UDP_SERVER_ADDR, UDP_SERVER_PORT, TCP_SERVER_ADDR, TCP_SERVER_PORT))
		return 0;

	udp2tcp.Run(UDP_THREADS);
	udp2tcp.Terminate();

	/////////////////////////////////////////////////
	return 0;
}

void OnExit(void)
{
	udp2tcp.Terminate();
	FSOCK_CLEANUP();		// Sockets cleanup
	std::cout << "Exit" << endl;
	system("pause");
}
