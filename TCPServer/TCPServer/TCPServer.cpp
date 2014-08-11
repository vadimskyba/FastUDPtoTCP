// TCPServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <inttypes.h>

#include <string>
#include <map>
#include <queue>

#include <thread>
#include <mutex> 
#include <condition_variable>

#include "CrossPlatform.h"

using namespace std;

struct UDP_MESSAGE{
	uint16_t MessageSize;
	uint8_t MessageType;
	uint64_t MessageId;
	uint64_t MessageData;
};
#define PORT_TCPSERVER	7777

int readn(SOCKET fd, char *bp, size_t len);
void OnExit(void);
bool bStopSig = false;
SOCKET s = 0;

#if defined _WIN32 || defined _WIN64 || __MINGW32__
int	_tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char **argv)
#endif
{
	FSOCK_INIT();

	struct sockaddr_in local;
	memset(&local, NULL, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = ::inet_addr("0.0.0.0");
	local.sin_port = ::htons(PORT_TCPSERVER);

	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (FSOCK_ISERROR(s))
		return 0;

	const int on = 1;
	::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)on, sizeof(on));

	if (::bind(s, (SOCKADDR*)&local, sizeof(local)))
		return -1;

	if (::listen(s, 5))
		return -1;

	struct sockaddr_in peer;
    socklen_t peerlen = sizeof(peer);

	cout << "TCP server is started now." << endl;

	while (!bStopSig)
	{
		SOCKET s1 = ::accept(s, (SOCKADDR*)&peer, &peerlen);

		if (FSOCK_ISERROR(s1))
		{
			FSOCK_CLOSE(s1);
			return false;
		}

		cout << "Client connected." << endl;

		do
		{
			UDP_MESSAGE msg;
			int rc = readn(s1, (LPSTR)&msg, sizeof(msg));

            if (rc == 0 || FSOCK_ISERROR(rc))
			{
				cout << "Client disconnected." << endl;
				FSOCK_CLOSE(s1);
				break;
			}

			if (rc == sizeof(msg))
				cout << "Received packet: " << msg.MessageId << endl;
			else
				cout << "msg error" << endl;

		} while (!bStopSig);
	}

	return 0;
}

int readn(SOCKET fd, char *bp, size_t len)
{
	int cnt;
	int rc;

	cnt = len;
	while (cnt > 0)
	{
		rc = recv(fd, bp, cnt, 0);

		if (rc < 0)	// read error
		{
			if (errno == EINTR)	// interrupted
				continue;		// restart the read
			return -1;			// return error
		}

		if (rc == 0)			// EOF?
			return len - cnt;	// return short count

		bp += rc;
		cnt -= rc;
	}

	return len;
}

void OnExit(void)
{
	bStopSig = true;
	FSOCK_CLOSE(s);
	FSOCK_CLEANUP();
	std::cout << "Exit" << endl;
	system("pause");
}

