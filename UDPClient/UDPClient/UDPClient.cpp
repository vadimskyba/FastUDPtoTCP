// UDPClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <inttypes.h>

#include <thread>
#include <random>

#include "CrossPlatform.h"

using namespace std;

struct UDP_MESSAGE{
	uint16_t MessageSize;
	uint8_t MessageType;
	uint64_t MessageId;
	uint64_t MessageData;
};
#define PORT_UDPSERVER	7777

bool bStopSig = false;
SOCKET s = 0;
void OnExit(void);

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
	local.sin_addr.s_addr = ::inet_addr("127.0.0.1");
	local.sin_port = ::htons(PORT_UDPSERVER);

	s = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (FSOCK_ISERROR(s))
	{
		FSOCK_CLOSE(s);
		return false;
	}

	std::mt19937 generator((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());

	while (!bStopSig)
	{
		UDP_MESSAGE msg;

		std::this_thread::sleep_for(std::chrono::milliseconds(100 + generator() % 900)); // delay: 100 - 999 ms

		static int i = 0;

		msg.MessageSize = sizeof(msg);
		msg.MessageType = 7777;
		msg.MessageId = i++;
		msg.MessageData = 9 + generator() % 3; // 9 - 11

		int rc = sendto(s, (LPCSTR)&msg, sizeof(msg), 0, (SOCKADDR*)&local, sizeof(local));
		
		cout << "Sended UDP packet: " << msg.MessageId;
		if (msg.MessageData == 10)
			cout << " (ID = 10)";
		cout << endl;
	}

	return 0;
}

void OnExit(void)
{
	bStopSig = true;
	FSOCK_CLOSE(s);
	FSOCK_CLEANUP();
	std::cout << "Exit" << endl;
	system("pause");
}