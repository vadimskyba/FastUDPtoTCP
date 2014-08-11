#pragma once

#if defined _WIN32 || defined _WIN64 || __MINGW32__

	#pragma comment(lib, "Ws2_32.lib") // compile with a library
	#include <windows.h>

	#define FSOCK_INIT()		::winsockinit()
	#define FSOCK_CLEANUP()		::WSACleanup()
	#define FSOCK_CLOSE(s)		::closesocket(s)
	#define FSOCK_ISERROR(s)	((s) == SOCKET_ERROR)

	#define socklen_t   int

	void winsockinit();
	int inet_aton(char *cp, struct in_addr *pin);

#else	// linux

	#include <netinet/tcp.h>
	#include <string.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <unistd.h>
    #include <sys/param.h>

	#define TRUE 1
	#define BOOL int
	#define SOCKET int
	#define SOCKADDR sockaddr
	#define LPCSTR const char *

                                // Prevents to stop program on SIGPIPE Broken pipe signal
    #define FSOCK_INIT()        signal(SIGPIPE, SIG_IGN)
	#define FSOCK_CLEANUP()
	#define FSOCK_CLOSE(s)		close(s)
	#define FSOCK_ISERROR(s)	((s) < 0)

#endif
